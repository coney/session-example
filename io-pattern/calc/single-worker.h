#ifndef __SINGLE_WORKER_H__
#define __SINGLE_WORKER_H__

class SingleWorker
{
public:
    int operator()(int fd) {
        printf("work with %d\n", fd);
        return 0;
    }
};

#endif