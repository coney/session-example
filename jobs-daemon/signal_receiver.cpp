#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig) {
    printf("receive signal %d\n", sig);
    fflush(stdout);
}

int main(int argc, char **argv) {

    for (int i = SIGHUP; i <= SIGUSR2; ++i) {
        signal(i, signal_handler);
    }

    while(1) {
        sleep(1000);
    }

    return 0;
}