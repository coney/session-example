#ifndef __CALC_H__
#define __CALC_H__

#include <unistd.h>
#include <signal.h>
#include <aio.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <boost/tr1/functional.hpp>


#ifdef NDEBUG
#define ensure(expr)    expr
#else
#define ensure          assert
#endif // NDEBUG

#define logdebug        printf

#define SOCKET_BUFSIZE  4096

#endif