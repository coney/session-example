#ifndef __MULTITHREAD_WORKER_H__
#define __MULTITHREAD_WORKER_H__

#include <unistd.h>
#include <semaphore.h>
#include <utility>
#include "single-worker.h"

class MultithreadWorker
{
public:
    MultithreadWorker(unsigned int max_worker_count = 10) {
        m_max_worker_count = max_worker_count;
        sem_init(&m_worker_sem, 0, max_worker_count);
    }
    int operator()(int fd) {
        sem_wait(&m_worker_sem);
        pthread_t thread;
        int ret = pthread_create(&thread, NULL, workThreadProc, 
            new std::pair<MultithreadWorker *, int>(this, fd));
        assert(ret == 0);
        ret = pthread_detach(thread);
        assert(ret == 0);
        return 0;
    }
private:
    static void *workThreadProc(void *arg) {
        std::pair<MultithreadWorker *, int> *params = (std::pair<MultithreadWorker *, int> *)arg;
        MultithreadWorker *worker = params->first;
        worker->process(params->second);
        return NULL;
    }

    void process(int fd) {
        // process with single worker
        SingleWorker()(fd);
        sem_post(&m_worker_sem);
    }


    unsigned int m_max_worker_count;
    sem_t        m_worker_sem;
};

#endif