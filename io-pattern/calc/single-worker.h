#ifndef __SINGLE_WORKER_H__
#define __SINGLE_WORKER_H__

#include "calc.h"

#include "app-socket.h"

class SingleWorker
{
public:
    int operator()(int fd) {
        AppSocket sock(fd, true);
        sock.receive();
        return 0;
    }
};

#endif