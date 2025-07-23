#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "log.h"

int main(int argc, char *argv[]) {
    int n = chroot(getenv("HOME"));
    if (n == -1) {
        log_error("chroot failed: %s", strerror(errno));
        exit(1);
    }

    n = chdir("/");
    if (n == -1) {
        log_error("chdir failed: %s", strerror(errno));
        exit(1);
    }

    DIR *dir = opendir("/");
    if (dir == NULL) {
        log_error("opendir failed: %s", strerror(errno));
        exit(1);
    }

    struct dirent *dirent;
    while ((dirent = readdir(dir)) != NULL) {
        switch (dirent->d_type) {
        case DT_REG:
            log_info("file: %s", dirent->d_name);
            continue;
        case DT_DIR:
            log_info("dir: %s", dirent->d_name);
            continue;
        default:
            log_info("dirent: %d %s", dirent->d_type, dirent->d_name);
            continue;
        }
    }

    n = closedir(dir);
    if (n == -1) {
        log_error("closedir failed: %s", strerror(errno));
        exit(1);
    }

    return 0;
}
