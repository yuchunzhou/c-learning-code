#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "log.h"

int main(int argc, char *argv[]) {
    int index = -1;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--enable_close_on_exec") == 0) {
            index = i;
            if (argc < index + 2) {
                log_error("missing --enable_close_on_exec argument value");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (index == -1) {
        log_error("missing --enable_close_on_exec argument");
        exit(EXIT_FAILURE);
    }

    const int enable_close_on_exec = atoi(argv[index + 1]);
    int fd;

    switch (enable_close_on_exec) {
    case 0:
        fd = open("./test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0755);
        break;
    case 1:
        // below new created process will throw segmentation fault
        fd = open("./test.txt", O_CREAT | O_WRONLY | O_TRUNC | O_CLOEXEC, 0755);
        break;
    default:
        log_info("invalid --enable_close_on_exec argument value");
        exit(EXIT_FAILURE);
    }

    if (fd == -1) {
        log_error("call open failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    const pid_t pid = fork();
    if (pid > 0) { // original process
        const int ret = close(fd);
        if (ret == -1) {
            log_error("call close failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        } else {
            exit(EXIT_SUCCESS);
        }
    } else if (pid == 0) { // new created process
        const char *data_buf = "hello world!\n";
        int n = write(fd, data_buf, strlen(data_buf));
        if (n == -1) {
            log_error("call write failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        log_info("process %d writes %d bytes data to file %d", getpid(), n, fd);

        char fd_arg[100] = {0};
        n = sprintf(fd_arg, "%d", fd);
        if (n < 0) {
            log_error("call sprintf failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        const char *args[] = {fd_arg, NULL};
        n = execve("./prog", (char **)args, NULL);
        if (n == -1) {
            log_error("call execve failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    } else if (pid == -1) {
        log_error("call fork failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
