#define _GNU_SOURCE

#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "log.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void log_lock_func(bool lock, void *udata) {
    if (lock) {
        pthread_mutex_lock(&mutex);
    } else {
        pthread_mutex_unlock(&mutex);
    }
}

void *thread_func(void *args) {
    int cpu_id;

    while (1) {
        cpu_id = sched_getcpu();
        log_info("current cpu id: %d", cpu_id);
        usleep(1000 * 200);
    }
}

int main(int argc, char *argv[]) {
    log_set_lock(log_lock_func, NULL);

    pthread_t thread = {0};
    int ret = pthread_create(&thread, NULL, thread_func, NULL);
    if (ret < 0) {
        log_error("failed to create thread: %s", strerror(errno));
        exit(1);
    }

    cpu_set_t cpu_set = {0};
    CPU_ZERO(&cpu_set);
    CPU_SET(3, &cpu_set);

    ret = pthread_setaffinity_np(thread, 1, &cpu_set);
    if (ret < 0) {
        log_error("failed to set cpu affinity for thread: %s", strerror(errno));
        exit(1);
    }
    log_info(">>> cpu affinity has been setted <<<");

    sleep(100);
    return 0;
}
