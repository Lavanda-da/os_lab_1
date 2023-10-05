#include "stdio.h"
#include "iostream"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"
#include "string"
#include "fcntl.h"

int main() {
    std::string fileName11 = "", fileName22 = "";
    char c;

    while (true) {
        read(STDIN_FILENO, &c, sizeof(char));
        if (c == '\n' || c == ' ') {
            break;
        }
        fileName11 += c;
    }
    char fileName1[fileName11.size() + 1];
    for (int i = 0; i < fileName11.size(); ++i) {
        fileName1[i] = fileName11[i];
    }
    fileName1[fileName11.size()] = '\0';

    while (true) {
        read(STDIN_FILENO, &c, sizeof(char));
        if (c == '\n' || c == ' ') {
            break;
        }
        fileName22 += c;
    }
    char fileName2[fileName22.size() + 1];
    for (int i = 0; i < fileName22.size(); ++i) {
        fileName2[i] = fileName22[i];
    }
    fileName2[fileName22.size()] = '\0';

    int pipe1_fd[2]; // 0 - read, 1 - write
    int err1 = pipe(pipe1_fd);
    if (err1 == -1) {
        perror("pipe");
        return -1;
    }
    int pipe2_fd[2];
    int err2 = pipe(pipe2_fd);
    if (err2 == -1) {
        perror("pipe");
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork1");
        return -1;
    }
    pid_t pid2;
    if (pid > 0) {
        pid2 = fork();
        if (pid2 == -1) {
            perror("fork2");
            return -1;
        } else if (pid2 == 0) {
            pid = 1;
        }
    }
    if (pid == 0) {
        pid2 = 1;
    }

    std::string s, s1, s2;
    char c1, c2;
    if (pid > 0 && pid2 > 0) {
        while (read(STDIN_FILENO, &c, sizeof(c))) {
            if (c == ' ' || c == '\n' || c == '\t') {
                s += '\n';
                if (s.size() % 2 == 0) {
                    s1 += s;
                } else {
                    s2 += s;
                }
                s = "";
                continue;
            }
            s += c;
        }
        close(pipe1_fd[0]);
        close(pipe2_fd[0]);
        write(pipe1_fd[1], s1.c_str(), s1.length());
        write(pipe2_fd[1], s2.c_str(), s2.length());
        close(pipe1_fd[1]);
        close(pipe2_fd[1]);
    }
    if (pid == 0) {
        close(pipe1_fd[1]);
        dup2(pipe1_fd[0], STDIN_FILENO);
        close(pipe1_fd[0]);
        execl("son1.out", "son1.out", fileName1, NULL);
    } else if (pid2 == 0) {
        close(pipe2_fd[1]);
        dup2(pipe2_fd[0], STDIN_FILENO);
        close(pipe2_fd[0]);
        execl("son2.out", "son2.out", fileName2, NULL);
    }
    
    return 0;
}
