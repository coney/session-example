#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#ifdef __APPLE__
#define NR_OPEN 1024
#else
#include <linux/limits.h>
#endif

void fork_process() {
    pid_t pid = fork();
    if (pid == -1)
        exit(-1);
    else if (pid != 0)
        exit(EXIT_SUCCESS);
}

void create_new_session() {
    if (setsid() == -1)
        exit(-1);
}

void change_current_directory() {
    if (chdir("/") == -1)
        exit(-1);
}

void close_opened_files() {
    for (int i = 0; i < NR_OPEN; i++)
        close(i);
}

void redirect_stdin_out_err() {
    /* stdin */
    open("/dev/null", O_RDWR);
    /* stdout */
    dup(0);
    /* stderror */
    dup(0);
}

void handle_signals() {
    signal(SIGHUP, SIG_IGN);
}

void run() {
    sleep(10000);
}


int main(int argc, char **argv) {

    /* create new process */
    fork_process();

    /* create new session and process group */
    create_new_session();

    /* handle signals */
    handle_signals();

    /* fork again to kill session leader */
    fork_process();

    /* set the working directory to the root directory */
    change_current_directory();

    /* close all open files--NR_OPEN is overkill, but works */
    close_opened_files();

    /* redirect fd's 0,1,2 to /dev/null */
    redirect_stdin_out_err();

    /* do its daemon thing... */
    run();

    return 0;
}