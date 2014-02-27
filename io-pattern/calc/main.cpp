#include "calc.h"
#include "calc-server.h"

#include "single-worker.h"
#include "multithread-worker.h"
#include "nonblock-worker.h"
#include "select-worker.h"
#include "epoll-worker.h"
#include "aio-worker.h"

int main(int argc, char **argv) {
    int opt;
    CalcServer::worker_t worker;
    while ((opt = getopt(argc, argv, "smnxea")) != -1)
    {
        switch (opt)
        {
        case 's':
            worker = SingleWorker();
            break;
        case 'm':
            worker = MultithreadWorker();
            break;
        case 'n':
            worker = NonblockWorker();
            break;
        case 'x':
            worker = SelectWorker();
            break;
        case 'e':
            worker = EPollWorker();
            break;
        case 'a':
            worker = AIOWorker();
            break;
        default:
            return -1;
        }
    }

    if (!worker) {
        worker = SingleWorker();
    }

    return CalcServer(worker).run();
}
