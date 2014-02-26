#ifndef __SINGLE_WORKER_H__
#define __SINGLE_WORKER_H__

#include <unistd.h>

#include "app-socket.h"

class SingleWorker
{
public:
    int operator()(int fd) {
        AppSocket sock(fd);
        sock.receive();
        sock.getApiSocket()->receive();
        return 0;
    }
};

#endif