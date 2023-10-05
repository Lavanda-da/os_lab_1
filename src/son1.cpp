#include "stdio.h"
#include "iostream"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"
#include "string"
#include "fcntl.h"

int main(int argc, char *argv[]) {
    int file;
    file = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
    char c;
    while (read(STDIN_FILENO, &c, sizeof(c)) > 0) {
        write(file, &c, sizeof(c));
    }
    close(file);
    return 0;
}
