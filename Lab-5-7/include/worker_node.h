#pragma once

#include <iostream>
#include <zmq.hpp>
#include <map>
#include <thread>
#include <unistd.h>
#include "utils.h"


class WorkerNode {
public:
    WorkerNode(int id, const std::string &address);

    ~WorkerNode();

    void run();

private:
    int id;
    zmq::context_t context;
    zmq::socket_t socket;
    std::string address;
    std::map<std::string, int> localDict;
    ChildInfo left = {0, 0, ""};
    ChildInfo right = {0, 0, ""};

    void processCommand(const std::string &command);
};
