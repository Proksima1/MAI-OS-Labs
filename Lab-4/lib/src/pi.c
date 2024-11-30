#include "pi.h"


double PiWallis(int K) {
    double result = 1;
    for (int i = 1; i <= K; ++i) {
        result *= (((2.0 * i) / (2.0 * i - 1)) * ((2.0 * i) / (2.0 * i + 1)));
    }
    return result * 2.0;
}

double PiLeibniz(int K) {
    double pi = 0.0;
    for (int i = 0; i < K; i++) {
        pi += (i % 2 == 0 ? 1.0 : -1.0) / (2.0 * i + 1.0);
    }
    return pi * 4.0;
}