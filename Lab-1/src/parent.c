#include "utils.h"
#include "sys/wait.h"
#include "stdlib.h"


int MainFunction(char *filename) {
    const int BUFFER_SIZE = 1024;
    int fileDescriptor = OpenFile(filename);
    int pipeDescriptor[2];
    CreatePipe(pipeDescriptor);
    pid_t pid = CreateProcess();

    if (pid == 0) {
        close(pipeDescriptor[READ_END]);
        dup2(fileDescriptor, STDIN_FILENO);
        dup2(pipeDescriptor[WRITE_END], STDOUT_FILENO);
        if (execl("./child", "child", NULL) == -1) {
            perror("Child run error");
        }
    } else {
        close(pipeDescriptor[WRITE_END]);
        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;
        while ((bytesRead = read(pipeDescriptor[READ_END], buffer, BUFFER_SIZE - 1)) > 0) {
            buffer[bytesRead] = '\0';
            char *line = strtok(buffer, "\n");
            while (line != NULL) {
                printf("%s\n", line);
                line = strtok(NULL, "\n");
            }
        }
        int status;
        waitpid(pid, &status, 0);
    }
    return EXIT_SUCCESS;
}