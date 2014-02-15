#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char **argv) {

    // fork child process, and exit parent process
    if (fork() > 0) {
        return 0;
    }

//    setsid();

    sleep(100000000);
    return 0;
}