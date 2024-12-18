#pragma once

#include <string>
#include <utility>
#include <vector>
#include <map>
#include <zmq.hpp>
#include <queue>
#include <utility>
#include "iostream"

inline zmq::context_t globalContext(1);


struct ChildInfo {
    int id;
    int pid;
    std::string address;
};


class WorkerNodeInfo {
public:
    int id;
    int pid;
    std::string address;
    WorkerNodeInfo *left = nullptr;
    WorkerNodeInfo *right = nullptr;
    WorkerNodeInfo *parent = nullptr;
public:
    WorkerNodeInfo(int id, int pid, std::string address) : id(id), pid(pid), address(std::move(address)) {};

    WorkerNodeInfo(int id, int pid, std::string address, WorkerNodeInfo *parent) : id(id), pid(pid),
                                                                                   address(std::move(address)),
                                                                                   parent(parent) {};

};


void sendResponse(zmq::socket_t &socket, const std::string &response);

std::string receiveRequest(zmq::socket_t &socket);

bool
sendRequestWithTimeout(zmq::socket_t &socket, const std::string &request, std::string &response, int timeout = 1000);

void createWorker(int id, ChildInfo &info);

bool isPidAlive(int pid);