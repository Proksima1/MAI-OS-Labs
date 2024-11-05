#pragma once

#include "iostream"
#include "vector"

struct ThreadData {
    std::vector<std::string> *pack;
    int *positive_results;
    int round_numbers;
};

void *MakeOneRound(void *arg);

std::vector<std::string> MakePack();

double Process(int THREADS_NUMBER, int ROUNDS_NUMBER);