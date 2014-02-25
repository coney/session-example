#include <stdio.h>
#include <stdlib.h>

#include "calc-server.h"

#include "single-worker.h"

int main(int argc, char **argv) {
    CalcServer server((SingleWorker()));
    server.run();
    return 0;
}
