#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "utils.h"
#include <sys/mman.h>
#include "semaphore.h"


int main() {
    const int BUFFER_SIZE = 1024 * 10;
    char buffer[BUFFER_SIZE];

    sem_t *semaphore_write = sem_open("/semaphore_write", 0);
    sem_t *semaphore_read = sem_open("/semaphore_read", 0);

    ssize_t bytes_read;
    char *endPtr;
    int number;
    char *ptr = mmap(0, BUFFER_SIZE, PROT_WRITE, MAP_SHARED, STDOUT_FILENO, 0);

    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        char *line = strtok(buffer, "\n");

        while (line != NULL) {
            sem_wait(semaphore_write);
            number = strtol(line, &endPtr, 10);
            if (*endPtr != '\0' && *endPtr != '\n') {
                fprintf(stderr, "Invalid number: %s\n", line);
            } else {
                if (number < 0 || IsPrime(number)) {
                    sprintf(ptr, "%s", "stop");
                    sem_post(semaphore_read);
                    break;
                }
                if (!IsPrime(number)) {
                    sprintf(ptr, "%d", number);
                    sem_post(semaphore_read);
                }
            }
            line = strtok(NULL, "\n");
        }
    }
    munmap(ptr, BUFFER_SIZE);
    sem_close(semaphore_read);
    sem_post(semaphore_write);
//    sem_close(semaphore_write);
    exit(EXIT_SUCCESS);
}
