#include <iostream>
#include <zmq.hpp>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include "controller.h"


void Controller::processCommand(const std::string &command) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(command);
    while (std::getline(tokenStream, token, ' ')) {
        tokens.push_back(token);
    }

    if (tokens[0] == "create") {
        int id = std::stoi(tokens[1]);
        if (left.pid == 0) {
            createWorker(id, left);
        } else if (right.pid == 0) {
            createWorker(id, right);
        } else {
            zmq::context_t context(1);
            zmq::socket_t workerSocket(context, ZMQ_REQ);
            workerSocket.set(zmq::sockopt::linger, 0);
            workerSocket.connect(left.address);

            std::string command = tokens[0] + " " + tokens[1] + " " + std::to_string(nowLevel);
            std::string response;
            if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response == "1") {
                std::cout << "Left child successfully added " << response << std::endl;
            } else {
                workerSocket.disconnect(left.address);
                workerSocket.connect(right.address);
                if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response == "1") {
                    std::cout << "Right child successfully added" << response << std::endl;
                } else {
                    std::cout << "Error: Both children can`t add new node on level" << nowLevel << std::endl;
                    nowLevel++;
                }
            }
        }
    } else if (tokens[0] == "exec") {
        int id = std::stoi(tokens[1]);

        if (left.pid == 0) {
            std::cout << "Error:" << id << ": Not found" << std::endl;
            return;
        }

        zmq::context_t context(1);
        zmq::socket_t workerSocket(context, ZMQ_REQ);
        workerSocket.set(zmq::sockopt::linger, 0);
        workerSocket.connect(left.address);

        std::string command = tokens[0] + " " + tokens[1];
        for (size_t i = 2; i < tokens.size(); ++i) {
            command += " " + tokens[i];
        }

        std::string response;
        if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response != "error not_found") {
            std::cout << response << std::endl;
        } else {
            workerSocket.disconnect(left.address);
            workerSocket.connect(right.address);
            if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response != "error not_found") {
                std::cout << response << std::endl;
            } else {
                std::cout << "Error:" << id << ": Node is unavailable" << std::endl;
            }
        }
        workerSocket.close();

    } else if (tokens[0] == "ping") {
        std::string id = tokens[1];
        if (left.id == stoi(id)) {
            if (isPidAlive(left.pid)) {
                std::cout << "Ok: 1" << std::endl;
            } else {
                std::cout << "Ok: 0" << std::endl;
            }
        } else if (right.id == stoi(id)) {
            if (isPidAlive(left.pid)) {
                std::cout << "Ok: 1" << std::endl;
            } else {
                std::cout << "Ok: 0" << std::endl;
            }
        } else {
            zmq::context_t context(1);
            zmq::socket_t workerSocket(context, ZMQ_REQ);
            workerSocket.set(zmq::sockopt::linger, 0);
            workerSocket.connect(left.address);
            std::string response;
            if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response != "-1") {
                std::cout << response << std::endl;
                if (response == "1") {
                    std::cout << "Ok: 1" << std::endl;
                } else {
                    std::cout << "Ok: 0" << std::endl;
                }
            } else {
                workerSocket.disconnect(left.address);
                workerSocket.connect(right.address);
                if (sendRequestWithTimeout(workerSocket, command, response, 1000) && response != "-1") {
                    std::cout << response << std::endl;
                    if (response == "1") {
                        std::cout << "Ok: 1" << std::endl;
                    } else {
                        std::cout << "Ok: 0" << std::endl;
                    }
                } else {
                    std::cout << "Error: Not found" << std::endl;
                }
            }
        }
    } else {
        std::cout << "Error: Unknown command" << std::endl;
    }
}

void Controller::run() {
    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        }
        processCommand(command);
    }
}