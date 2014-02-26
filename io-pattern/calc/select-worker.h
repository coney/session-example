#ifndef __SELECT_WORKER_H__
#define __SELECT_WORKER_H__

#include <unistd.h>
#include <map>
#include <pthread.h>
#include <sys/select.h>
#include <list>
#include "app-socket.h"
#include <functional>
#include <algorithm>

class SelectWorker {
public:
    SelectWorker() {
        m_this = this;
        pthread_t thread;
        pthread_mutex_init(&m_sockets_guard, NULL);
        pthread_create(&thread, NULL, pollThreadProc, this);
        pthread_detach(thread);
    }

    int operator()(int fd) {
        m_this->addSocket(fd);
        return 0;
    }

private:

    static void *pollThreadProc(void *arg) {
        SelectWorker *worker = (SelectWorker *)arg;
        while (1) {
            worker->poll();
        }
        return NULL;
    }

    void addSocket(int fd) {
        AppSocket *socket = new AppSocket(fd);

        // block socket should work here, but select don't 
        // promise us that we can get data on read events
        socket->setNonBlock();
        socket->getApiSocket()->setNonBlock();

        // add app fd and api fd into m_fds
        // and build mapping in m_sockets
        pthread_mutex_lock(&m_sockets_guard);
        //printf("add socket %d %d\n", socket->getFd(),
        //    socket->getApiSocket()->getFd());
        m_sockets[socket->getFd()] = socket;
        m_sockets[socket->getApiSocket()->getFd()] = socket->getApiSocket();
        pthread_mutex_unlock(&m_sockets_guard);
    }

    void removeSocket(AppSocket *socket) {
        m_sockets.erase(socket->getFd());
        m_sockets.erase(socket->getApiSocket()->getFd());
        //printf("remove socket %d %d\n", socket->getFd(),
        //    socket->getApiSocket()->getFd());
        delete socket;
    }

    void poll() {
        fd_set read_fds;
        FD_ZERO(&read_fds);

        // prepare fdset to select
        pthread_mutex_lock(&m_sockets_guard);
        int nfds = 0;
        std::for_each(m_sockets.begin(), m_sockets.end(), 
            [&](std::pair<const int, HttpSocket*>& sockinfo){
                FD_SET(sockinfo.first, &read_fds);
                nfds = std::max(nfds, sockinfo.first);
        });
        pthread_mutex_unlock(&m_sockets_guard);

        if (!nfds) {
            return;
        }

        // timeout is 1ms
        struct timeval timeout = { 0, 1000 };
        int ret = select(nfds + 1, &read_fds, NULL, NULL, &timeout);
        assert(ret >= 0);

        // if there is something to read
        if (ret > 0) {
            std::list<AppSocket *> delete_pendings;

            pthread_mutex_lock(&m_sockets_guard);

            std::for_each(m_sockets.begin(), m_sockets.end(), 
                [&](std::pair<const int, HttpSocket*>& it){
                    int fd = it.first;
                    HttpSocket *socket = it.second;

                    // test if this fd is available
                    if (FD_ISSET(fd, &read_fds))
                    {
                        ret =  socket->receive();
                        AppSocket *appSocket = dynamic_cast<AppSocket *>(socket);
                        if (appSocket == NULL) {
                            appSocket = dynamic_cast<ApiSocket *>(socket)->getAppSocket();
                        }

                        if (appSocket->receiveDone() && appSocket->getApiSocket()->receiveDone())
                        {
                            // remove all finished sockets
                            delete_pendings.push_back(appSocket);
                        }
                    }
            });
            std::for_each(delete_pendings.begin(), delete_pendings.end(),
                bind1st(std::mem_fun(&SelectWorker::removeSocket), this));

            pthread_mutex_unlock(&m_sockets_guard);
        }
    }

    pthread_mutex_t m_sockets_guard;
    std::map<int, HttpSocket *> m_sockets;

    // strange behavior for tr1::functional;
    SelectWorker *m_this;
};

#endif