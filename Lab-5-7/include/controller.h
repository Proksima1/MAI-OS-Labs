#pragma once
#include "utils.h"


class Controller {
private:
    ChildInfo left = {0, 0, ""};
    ChildInfo right = {0, 0, ""};
    int nowLevel = 1;
private:
    void processCommand(const std::string &command);

public:
    void run();
};