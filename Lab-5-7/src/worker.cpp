#include <iostream>
#include "worker_node.h"


int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <id> <address>" << std::endl;
        return 1;
    }
    int id = std::stoi(argv[1]);
    std::string address = argv[2];
    WorkerNode worker(id, address);
    worker.run();
    return 0;
}