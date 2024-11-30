#include "gcd.h"

int GCD_Euclid(int A, int B) {
    while (B != 0) {
        int temp = B;
        B = A % B;
        A = temp;
    }
    return A;
}

int GCD_Naive(int A, int B) {
    int gcf = 1;
    for (int i = 1; i <= A && i <= B; i++) {
        if (A % i == 0 && B % i == 0) {
            gcf = i;
        }
    }
    return gcf;
}