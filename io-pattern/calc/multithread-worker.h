#ifndef __MULTITHREAD_WORKER_H__
#define __MULTITHREAD_WORKER_H__

#include "calc.h"
#include "single-worker.h"

class MultithreadWorker
{
public:
    MultithreadWorker(unsigned int max_worker_count = 10) {
        m_max_worker_count = max_worker_count;
        sem_init(&m_worker_sem, 0, max_worker_count);
    }

    int operator()(int fd) {
        // avaliable workers count --
        sem_wait(&m_worker_sem);

        pthread_t thread;
        pthread_create(&thread, NULL, [&](void *arg){
            std::pair<MultithreadWorker *, int> *params = (std::pair<MultithreadWorker *, int> *)arg;
            MultithreadWorker *worker = params->first;

            worker->process(params->second);

            delete params;
            return (void *)NULL;
        }, new std::pair<MultithreadWorker *, int>(this, fd));
        pthread_detach(thread);

        return 0;
    }

private:
    void process(int fd) {
        // process with single worker
        SingleWorker()(fd);

        // avaliable workers count ++
        sem_post(&m_worker_sem);
    }

    unsigned int m_max_worker_count;
    sem_t        m_worker_sem;
};

#endif