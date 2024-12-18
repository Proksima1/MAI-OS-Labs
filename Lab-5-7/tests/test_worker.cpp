#include <gtest/gtest.h>
#include <zmq.hpp>
#include <thread>
#include <chrono>
#include "utils.h"
#include "worker_node.h"

TEST(WorkerTest, ExecCommand) {
    int id = 1;
    std::string address = "tcp://127.0.0.1:5556";
    pid_t pid = fork();
    if (pid == 0) {
        if (execl("./worker", "./worker", std::to_string(id).c_str(), address.c_str(), NULL) == -1) {
            perror("Child run error");
        }
    }

    std::string command = "exec 1 MyVar 10";
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(address);

    sendResponse(socket, command);
    std::string response = receiveRequest(socket);

    EXPECT_EQ(response, "ok 1");

    command = "exec 1 MyVar";
    sendResponse(socket, command);
    response = receiveRequest(socket);
    EXPECT_EQ(response, "ok 10");


    kill(pid, SIGTERM);
}