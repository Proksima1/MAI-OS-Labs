#include "stdio.h"
#include "parent.h"



int main() {
    char *filename = NULL;
    size_t _ = 0;
    getline(&filename, &_, stdin);
    MainFunction(filename);
}