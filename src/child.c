#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "utils.h"

const int BUFFER_SIZE = 1024;

int main() {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    char *endPtr;
    int number;
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        char *line = strtok(buffer, "\n");
        while (line != NULL) {
            number = strtol(line, &endPtr, 10);
            if (*endPtr != '\0' && *endPtr != '\n') {
                fprintf(stderr, "Invalid number: %s\n", line);
            } else {
                if (number < 0 || IsPrime(number)) {
                    exit(0);
                }
                if (!IsPrime(number)) {
                    printf("%d\n", number);
                }
            }
            line = strtok(NULL, "\n");
        }
    }
    exit(EXIT_SUCCESS);
}
