#include "utils.h"
#include "sys/wait.h"
#include "stdlib.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>


int MainFunction(char *filename) {
    const int BUFFER_SIZE = 1024, SHM_SIZE = 1024 * 10;
    sem_unlink("/semaphore_write");
    sem_unlink("/semaphore_read");

    sem_t *semaphore_write = sem_open("/semaphore_write", O_CREAT, 0666, 0);
    sem_t *semaphore_read = sem_open("/semaphore_read", O_CREAT, 0666, 0);

    if (semaphore_read == SEM_FAILED) {
        perror("Unable to create semaphore");
        exit(EXIT_FAILURE);
    }
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
        char *ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmDescriptor, 0);

        sem_post(semaphore_write);

        while (strcmp(ptr, "stop") != 0) {
            sem_wait(semaphore_read);

            if (strcmp(ptr, "") != 0 && strcmp(ptr, "stop") != 0) {
                printf("%s\n", ptr);
            }
            sem_post(semaphore_write);

        }
        munmap(ptr, BUFFER_SIZE);
        int status;
        waitpid(pid, &status, 0);
    }
    sem_close(semaphore_read);
    sem_close(semaphore_write);
    sem_unlink("/semaphore_write");
    sem_unlink("/semaphore_read");
    close(shmDescriptor);
    shm_unlink("/virtual_memory");
    close(fileDescriptor);
    return EXIT_SUCCESS;
}