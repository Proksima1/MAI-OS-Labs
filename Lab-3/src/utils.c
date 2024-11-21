#include "utils.h"
#include "fcntl.h"
#include <sys/mman.h>
#include <unistd.h>

int IsPrime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void TrimString(char *str) {
    size_t start = 0, end = strlen(str) - 1;

    while (isspace(str[start])) {
        start++;
    }

    while (end > start && isspace(str[end])) {
        end--;
    }

    if (start > 0 || end < (strlen(str) - 1)) {
        memmove(str, str + start, end - start + 1);
        str[end - start + 1] = '\0';
    }
}


int CreateProcess(void) {
    pid_t pid = fork();
    if (-1 == pid) {
        perror("fork");
        exit(-1);
    }
    return pid;
}


int OpenSHM(char *filename, int size) {
    int fd = shm_open(filename, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, size);
    return fd;
}

int OpenFile(char *filename) {
    TrimString(filename);
    int fd = open(filename, O_RDONLY);
    return fd;
}


char *ReadString(FILE *stream) {
    if (feof(stream)) {
        return NULL;
    }

    const int chunkSize = 256;
    char *buffer = (char *) malloc(chunkSize);
    int bufferSize = chunkSize;

    if (!buffer) {
        printf("Couldn't allocate buffer");
        exit(EXIT_FAILURE);
    }

    int readChar;
    int idx = 0;
    while ((readChar = getc(stream)) != EOF) {
        buffer[idx++] = readChar;

        if (idx == bufferSize) {
            buffer = realloc(buffer, bufferSize + chunkSize);
            bufferSize += chunkSize;
        }

        if (readChar == '\n') {
            break;
        }
    }

    buffer[idx] = '\0';

    return buffer;
}