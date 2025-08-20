#include <aio.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

const static char *test_file = "test.txt";

void read_data_finished(__sigval_t sv) { log_info("data is ready ..."); }

int main(int argc, char *argv[]) {
    log_set_lock(log_lock_func, NULL);

    int fd = open(test_file, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, 0755);
    if (fd == -1) {
        log_error("open file failed: %s", strerror(errno));
        exit(1);
    }

    char *data = "hello world";
    int ret = write(fd, data, strlen(data));
    if (ret == -1) {
        log_error("write data to test file failed: %s", strerror(errno));
        exit(1);
    }
    log_info("write %d bytes data to file", ret);

    struct aiocb aio_cb;
    memset(&aio_cb, 0, sizeof(struct aiocb));

    aio_cb.aio_fildes = fd;
    aio_cb.aio_lio_opcode = LIO_READ;

    char buf[1024] = {0};
    aio_cb.aio_buf = buf;
    aio_cb.aio_nbytes = 1024;
    aio_cb.aio_reqprio = 0;

    struct sigevent sig_event;
    memset(&sig_event, 0, sizeof(struct sigevent));

    sig_event.sigev_notify = SIGEV_THREAD;
    sig_event._sigev_un._sigev_thread._function = read_data_finished;
    sig_event._sigev_un._sigev_thread._attribute = NULL;
    aio_cb.aio_sigevent = sig_event;

    ret = aio_read(&aio_cb);
    if (ret == -1) {
        log_error("aio read failure: %s", strerror(errno));
        exit(1);
    }

    const struct aiocb *const aio_cb_array[1] = {&aio_cb};

    struct timespec timeout;
    memset(&timeout, 0, sizeof(struct timespec));

    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;

    ret = aio_suspend(aio_cb_array,
                      sizeof(aio_cb_array) / sizeof(aio_cb_array[0]), &timeout);
    if (ret == -1) {
        log_error("aio suspend failure: %s", strerror(errno));
        exit(1);
    }

    ret = aio_return((struct aiocb *)aio_cb_array[0]);
    log_info("read %d bytes data:", ret);
    log_info(">>> %s <<<", buf);

    ret = close(fd);
    if (ret == -1) {
        log_error("close file failed: %s", strerror(errno));
        exit(1);
    }

    sleep(3);

    return 0;
}
