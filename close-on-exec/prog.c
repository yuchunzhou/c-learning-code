#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "log.h"

int main(int argc, char *argv[]) {
    const int fd = atoi(argv[0]);
    const char *data_buf = "goodbye world!\n";

    const int n = write(fd, data_buf, strlen(data_buf));
    if (n == -1) {
        log_error("call write failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    log_info("process %d writes %d bytes data to file %d", getpid(), n, fd);

    const int ret = close(fd);
    if (ret == -1) {
        log_error("call close failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
