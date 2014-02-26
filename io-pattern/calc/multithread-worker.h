#ifndef __MULTITHREAD_WORKER_H__
#define __MULTITHREAD_WORKER_H__

#include <unistd.h>

#include "single-worker.h"

class MultithreadWorker
{
public:
    MultithreadWorker() {
        signal(SIGCHLD, SIG_IGN);
    }
    int operator()(int fd) {
        pthread_t thread;
        int ret = pthread_create(&thread, NULL, workThreadProc, (void *)(long)fd);
        assert(ret == 0);
        ret = pthread_detach(thread);
        assert(ret == 0);
        return 0;
    }
private:
    static void *workThreadProc(void *arg) {
        return (void*)(long)SingleWorker()((int)(long)arg);
    }
};

#endif