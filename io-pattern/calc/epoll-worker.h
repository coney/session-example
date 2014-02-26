#ifndef __EPOLL_WORKER_H__
#define __EPOLL_WORKER_H__

#include <unistd.h>
#include <map>
#include <pthread.h>
#include <sys/select.h>
#include <list>
#include "app-socket.h"
#include <functional>
#include <algorithm>
#include <sys/epoll.h>

class EPollWorker {
public:
    EPollWorker() {
        m_this = this;

        m_epfd = epoll_create(100);
        assert(m_epfd > 0);

        pthread_t thread;
        pthread_create(&thread, NULL, pollThreadProc, this);
        pthread_detach(thread);
    }

    int operator()(int fd) {
        m_this->addSocket(fd);
        return 0;
    }

private:

    static void *pollThreadProc(void *arg) {
        EPollWorker *worker = (EPollWorker *)arg;
        while (1) {
            worker->poll();
        }
        return NULL;
    }

    int epollAdd(HttpSocket *socket) {
        struct epoll_event event;
        event.events = EPOLLIN | EPOLLRDHUP;
        event.data.ptr = socket;
        int ret = epoll_ctl(m_epfd, EPOLL_CTL_ADD, socket->getFd(), &event);
        printf("epoll add fd:%d errno:%d ret:%d socket:%p\n",socket->getFd(),
            errno, ret, socket);
        assert(ret == 0);
        return 0;
    }

    int epollRemove(HttpSocket *socket) {
        int ret = epoll_ctl(m_epfd, EPOLL_CTL_DEL, socket->getFd(), NULL);
        assert(ret == 0);
        return ret;
    }

    void addSocket(int fd) {
        AppSocket *socket = new AppSocket(fd);

        // in epoll lt mode, socket can block
        //socket->setNonBlock();
        //socket->getApiSocket()->setNonBlock();

        epollAdd(socket);
        epollAdd(socket->getApiSocket());
    }

    void removeSocket(AppSocket *socket) {
        epollRemove(socket->getApiSocket());
        epollRemove(socket);
        delete socket;
    }

    void poll() {
        const unsigned int eventCount = 8;
        struct epoll_event events[eventCount]; 
        int ret = epoll_wait(m_epfd, events, eventCount, 1000);
        assert(ret >= 0);
        if (ret > 0)
        {
            printf("%d sockets is ready\n", ret);
            for (unsigned int i = 0; i < (unsigned int)ret; ++i)
            {
                struct epoll_event &event = events[i];
                //printf("events %x on socket:%p\n", event.events, event.data.ptr);
                // just ignore EPOLLHUP
                if ((event.events & EPOLLIN))
                {
                    HttpSocket *socket = (HttpSocket *)event.data.ptr;
                    socket->receive();

                    AppSocket *appSocket = dynamic_cast<AppSocket *>(socket);
                    if (appSocket == NULL) {
                        appSocket = dynamic_cast<ApiSocket *>(socket)->getAppSocket();
                    }

                    if (appSocket->receiveDone() && appSocket->getApiSocket()->receiveDone())
                    {
                        // remove all finished sockets
                        removeSocket(appSocket);
                    }
                }
            }
        }
    }

    // strange behavior for tr1::functional;
    EPollWorker *m_this;
    int         m_epfd;
};

#endif