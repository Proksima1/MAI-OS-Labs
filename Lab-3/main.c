#include "stdio.h"
#include "parent.h"
#include "stdlib.h"
#include "string.h"
#include "utils.h"


int main() {
    char *filename = NULL;
    size_t length = 0;
    getline(&filename, &length, stdin);
    TrimString(filename);
    MainFunction(filename);
    free(filename);
}