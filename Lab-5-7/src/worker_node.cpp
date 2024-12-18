#include "worker_node.h"
#include "chrono"

WorkerNode::WorkerNode(int id, const std::string &address) : id(id), address(address) {
    context = zmq::context_t(1);
    socket = zmq::socket_t(context, ZMQ_REP);
    socket.bind(address);
}

void WorkerNode::processCommand(const std::string &command) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(command);
    while (std::getline(tokenStream, token, ' ')) {
        tokens.push_back(token);
    }

    if (tokens[0] == "exec") {
        if (tokens.size() == 3) {
            std::string requestedId = tokens[1];
            std::string key = tokens[2];
            if (this->id == stoi(requestedId)) {
                std::string data;
                if (localDict.find(key) != localDict.end()) {
                    data = "ok value " + std::to_string(localDict[key]);
                } else {
                    data = "ok value not_found";
                }
                sendResponse(socket, data);
                return;
            }
            if (left.pid == 0) {
                sendResponse(socket, "error not_found");
                return;
            }
            zmq::socket_t workerSocket(context, ZMQ_REQ);
            workerSocket.set(zmq::sockopt::linger, 0);
            workerSocket.connect(left.address);
            std::string response;
            if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response != "error not_found") {
                sendResponse(socket, response);
            } else {
                if (right.pid == 0) {
                    sendResponse(socket, "error not_found");
                    return;
                }
                workerSocket.disconnect(left.address);
                workerSocket.connect(right.address);
                if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response != "error not_found") {
                    sendResponse(socket, response);
                } else {
                    sendResponse(socket, "error not_found");
                }
            }
        } else if (tokens.size() == 4) {
            int requestedId = stoi(tokens[1]);
            if (requestedId == this->id) {
                std::string key = tokens[2];
                int value = std::stoi(tokens[3]);
                localDict[key] = value;
                sendResponse(socket, "ok id " + std::to_string(id));
            } else {
                if (left.pid == 0) {
                    sendResponse(socket, "error not_found");
                    return;
                }
                zmq::socket_t workerSocket(context, ZMQ_REQ);
                workerSocket.set(zmq::sockopt::linger, 0);
                workerSocket.connect(left.address);
                std::string response;
                if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response != "error not_found") {
                    sendResponse(socket, response);
                } else {
                    if (right.pid == 0) {
                        sendResponse(socket, "error not_found");
                        return;
                    }
                    workerSocket.disconnect(left.address);
                    workerSocket.connect(right.address);
                    if (sendRequestWithTimeout(workerSocket, command, response, 1000) &&
                        response != "error not_found") {
                        sendResponse(socket, response);
                    } else {
                        sendResponse(socket, "error not_found");
                    }
                }
            }

        }
    } else if (tokens[0] == "create") {
        int newId = std::stoi(tokens[1]);
        int nowLevel = std::stoi(tokens[2]) - 1;
        if (nowLevel == 0) {
            if (left.pid == 0) {
                createWorker(newId, left);
                sendResponse(socket, "1");
            } else if (right.pid == 0) {
                createWorker(newId, right);
                sendResponse(socket, "1");
            } else {
                sendResponse(socket, "0");
            }
        } else {
            std::string response;
            std::string newCommand = tokens[0] + " " + tokens[1] + " " + std::to_string(nowLevel);
            zmq::socket_t workerSocket(context, ZMQ_REQ);
            workerSocket.set(zmq::sockopt::linger, 0);
            workerSocket.connect(left.address);
            if (sendRequestWithTimeout(workerSocket, newCommand, response, 1000) && response == "1") {
                sendResponse(socket, response);
            } else {
                workerSocket.disconnect(left.address);
                workerSocket.connect(right.address);
                if (sendRequestWithTimeout(workerSocket, newCommand, response, 1000) && response == "1") {
                    sendResponse(socket, response);
                } else {
                    sendResponse(socket, "0");
                }
            }
            workerSocket.close();
        }
    } else if (tokens[0] == "ping") {
        int id = stoi(tokens[1]);
        if (left.id == id) {
            if (isPidAlive(left.pid)) {
                sendResponse(socket, std::to_string(1));
            } else {
                sendResponse(socket, std::to_string(0));
            }
        } else if (right.id == id) {
            if (isPidAlive(right.pid)) {
                sendResponse(socket, std::to_string(1));
            } else {
                sendResponse(socket, std::to_string(0));
            }
        } else {
            if (left.pid == 0) {
                sendResponse(socket, "-1");
                return;
            }
            zmq::socket_t workerSocket(context, ZMQ_REQ);
            workerSocket.set(zmq::sockopt::linger, 0);
            workerSocket.connect(left.address);
            std::string response;
            if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response != "-1") {
                sendResponse(socket, response);
            } else {
                if (right.pid == 0) {
                    sendResponse(socket, "-1");
                    return;
                }
                workerSocket.disconnect(left.address);
                workerSocket.connect(right.address);
                if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response != "-1") {
                    sendResponse(socket, response);
                } else {
                    sendResponse(socket, "-1");
                }
            }
            workerSocket.close();
        }
    }
}

void WorkerNode::run() {
    const int duration_minutes = 3;
    const int duration_seconds = duration_minutes * 60;

    auto start = std::chrono::steady_clock::now();
    while (true) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed_seconds >= duration_seconds) {
            break;
        }
        std::string command = receiveRequest(socket);
        processCommand(command);
    }
}

WorkerNode::~WorkerNode() {
    socket.unbind(address);
}
