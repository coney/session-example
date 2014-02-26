#include <stdio.h>
#include <stdlib.h>

#include "calc-server.h"

#include "single-worker.h"
#include "multithread-worker.h"
#include "nonblock-worker.h"

int main(int argc, char **argv) {
    int opt;
    CalcServer::worker_t worker;
    while ((opt = getopt(argc, argv, "smn")) != -1)
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
        default:
            break;
        }
    }

    if (!worker) {
        worker = SingleWorker();
    }
    
    return CalcServer(worker).run();
}
