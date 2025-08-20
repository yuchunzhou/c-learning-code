#include <stddef.h>

#include "log.h"

int main(int argc, char *argv[]) {
    struct Foo {
        char a;
        short b;
        long int c;
    };
    log_info("offset of b: %ld", offsetof(struct Foo, b));
    log_info("offset of c: %ld", offsetof(struct Foo, c));

    struct Foo foo;
    *((char *)&foo + offsetof(struct Foo, c)) = 2;
    log_info("c: %ld", foo.c);

    return 0;
}
