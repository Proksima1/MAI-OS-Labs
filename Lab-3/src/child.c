#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "utils.h"
#include <sys/mman.h>


int main() {
    const int BUFFER_SIZE = 1024 * 10;
    char buffer[BUFFER_SIZE], output[1000] = "";
    ssize_t bytes_read;
    char *endPtr;
    int number;
    char *ptr = mmap(0, BUFFER_SIZE, PROT_WRITE, MAP_SHARED, STDOUT_FILENO, 0);
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        char *line = strtok(buffer, "\n");
        while (line != NULL) {
            number = strtol(line, &endPtr, 10);
            if (*endPtr != '\0' && *endPtr != '\n') {
                fprintf(stderr, "Invalid number: %s\n", line);
            } else {
                if (number < 0 || IsPrime(number)) {
                    sprintf(ptr, "%s", output);
                    exit(EXIT_SUCCESS);
                }
                if (!IsPrime(number)) {
                    sprintf(output, "%s%d\n", output, number);
                }
            }
            line = strtok(NULL, "\n");
        }
    }
    exit(EXIT_SUCCESS);
}
