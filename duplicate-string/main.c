#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>

#include "log.h"

int main(int argc, char *argv[]) {
    char *str = "hello world!";
    log_info("string len: %ld", strlen(str));
    log_info("string addr: 0x%lx", str);

    // target string is copied to heap
    char *str1 = strdup(str);
    log_info("str1: %s, last char: [%d], str1: 0x%lx", str1,
             *(str1 + strlen(str)), str1);

    char *str2 = strndup(str, strlen(str));
    log_info("str2: %s, last char: [%d], str2: 0x%lx", str2,
             *(str2 + strlen(str)), str2);

    free(str1);
    str1 = NULL;

    free(str2);
    str2 = NULL;

    // target string is on stack
    char *str3 = strdupa(str);
    log_info("str3: %s last char: [%d], str3: 0x%lx", str3,
             *(str3 + strlen(str)), str3);

    char *str4 = strndupa(str, strlen(str));
    log_info("str4: %s last char: [%d], str4: 0x%lx", str4,
             *(str4 + strlen(str)), str4);

    return 0;
}
