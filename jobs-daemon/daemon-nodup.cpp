#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    for (int i = 0; i <= STDERR_FILENO; i++)
        close(i);

    int fd0 = open("/dev/null", O_RDWR);
    assert(fd0 == 0);

    FILE *logfile = fopen("log.txt", "w+");
    if (!logfile) {
        return -1;
    }

    fprintf(logfile, "log file fd is %d\n", fileno(logfile));
    fflush(logfile);

    system("./daemon-nohup.sh");

    fprintf(logfile, "daemon exit\n");
    fclose(logfile);

    return 0;
}