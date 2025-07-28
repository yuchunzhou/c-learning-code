#include <alloca.h>
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

#include "log.h"

void print_stack_size_limit() {
    struct rlimit rlimit = {0};

    int ret = getrlimit(RLIMIT_STACK, &rlimit);
    if (ret == -1) {
        log_error("call getrlimit failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    log_info(
        "stack soft limit size: %ld bytes, stack hard limit size: %ld bytes",
        rlimit.rlim_cur, rlimit.rlim_max);
}

int main(int argc, char *argv[]) {
    print_stack_size_limit();

    const size_t size = 1024 * 1024 * 1;
    char *buf = alloca(size);
    memset(buf, 0, size);

    log_info("allocate %ld bytes memory on stack", size);

    return EXIT_SUCCESS;
}
