#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        int random_num = rand();
        printf("new random number is generated: %d\n", random_num);
        sleep(1);
    }

    return 0;
}
