
#include <sys/wait.h>
#include "utils.h"

void sendResponse(zmq::socket_t &socket, const std::string &response) {
    zmq::message_t reply(response.size());
    memcpy(reply.data(), response.c_str(), response.size());
    socket.send(reply, zmq::send_flags::none);
}

std::string receiveRequest(zmq::socket_t &socket) {
    zmq::message_t request;
    socket.recv(request, zmq::recv_flags::none);
    return std::string(static_cast<char *>(request.data()), request.size());
}

bool sendRequestWithTimeout(zmq::socket_t &socket, const std::string &request, std::string &response, int timeout) {
    zmq::message_t req(request.size());
    memcpy(req.data(), request.c_str(), request.size());
    socket.send(req, zmq::send_flags::none);

    zmq::pollitem_t items[] = {{socket, 0, ZMQ_POLLIN, 0}};
    zmq::poll(&items[0], 1, std::chrono::milliseconds(timeout));

    if (items[0].revents & ZMQ_POLLIN) {
        response = receiveRequest(socket);
        return true;
    } else {
        return false;
    }
}


void createWorker(int id, ChildInfo &info) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(5556 + id);
    pid_t pid = fork();
    if (pid == 0) {
        if (execl("./worker", "./worker", std::to_string(id).c_str(), address.c_str(), NULL) == -1) {
            perror("Child run error");
        }
    } else if (pid > 0) {
        info = {id, pid, address};
        std::cout << "Ok: pid: " << pid << " port: " << std::to_string(5556 + id) << std::endl;
    } else {
        std::cout << "Error: Fork failed" << std::endl;
    }
}

bool isPidAlive(int pid) {
    int status = 0;
    int result = waitpid(pid, &status, WNOHANG);
    if (result == 0) {
        return true;
    } else {
        return false;
    }
}