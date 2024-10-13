#include "stdio.h"
#include "parent.h"
#include "stdlib.h"



int main() {
    char *filename = NULL;
    size_t length = 0;
    getline(&filename, &length, stdin);
    MainFunction(filename);
    free(filename);
}