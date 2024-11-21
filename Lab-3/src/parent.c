#include "utils.h"
#include "sys/wait.h"
#include "stdlib.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


int MainFunction(char *filename) {
    const int BUFFER_SIZE = 1024, SHM_SIZE = 1024 * 10;
    int shmDescriptor = OpenSHM("/virtual_memory", SHM_SIZE);
    if (shmDescriptor == -1) {
        perror("Unable create virtual file");
        exit(EXIT_FAILURE);
    }
    int fileDescriptor = OpenFile(filename);
    if (fileDescriptor == -1) {
        perror("Unable open the file");
        exit(EXIT_FAILURE);
    }

    pid_t pid = CreateProcess();

    if (pid == 0) {
        dup2(fileDescriptor, STDIN_FILENO);
        dup2(shmDescriptor, STDOUT_FILENO);
        if (execl("./child", "child", NULL) == -1) {
            perror("Child run error");
        }
    } else {
        char buffer[SHM_SIZE];
        char *ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shmDescriptor, 0);
        strcpy(buffer, ptr);
        char *line = strtok(buffer, "\n");
        while (line != NULL) {
            printf("%s\n", line);
            line = strtok(NULL, "\n");
        }
        munmap(ptr, BUFFER_SIZE);
        int status;
        waitpid(pid, &status, 0);
    }
    close(shmDescriptor);
    shm_unlink("/virtual_memory");
    close(fileDescriptor);
    return EXIT_SUCCESS;
}