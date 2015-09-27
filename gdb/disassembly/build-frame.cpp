#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

    int sum(int val) {
        if (val == 1) {
            return 1;
        }
        return val + sum(val - 1);
    }


    int main(int argc, char **argv) {

        printf("sum(4)=%d\n", sum(4));

        return 0;
    }
