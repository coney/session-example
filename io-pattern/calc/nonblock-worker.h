#ifndef __NONBLOCK_WORKER_H__
#define __NONBLOCK_WORKER_H__

#include <unistd.h>
#include <set>
#include <pthread.h>
#include <errno.h>
#include "app-socket.h"

class NonblockWorker {
public:
    NonblockWorker() {
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
        NonblockWorker *worker = (NonblockWorker *)arg;
        while (1) {
            worker->poll();
            usleep(1000000);
        }
        return NULL;
    }

    void addSocket(int fd) {
        AppSocket *socket = new AppSocket(fd);
        socket->setNonBlock();
        socket->getApiSocket()->setNonBlock();

        pthread_mutex_lock(&m_sockets_guard);
        m_sockets.insert(socket);
        pthread_mutex_unlock(&m_sockets_guard);
    }

    void removeSocket(AppSocket *socket) {
        pthread_mutex_lock(&m_sockets_guard);
        m_sockets.erase(socket);
        pthread_mutex_unlock(&m_sockets_guard);
        delete socket;
    }

    void poll() {
        pthread_mutex_lock(&m_sockets_guard);
        std::set<AppSocket *> sockets = m_sockets;
        pthread_mutex_unlock(&m_sockets_guard);

        for (std::set<AppSocket *>::iterator it = sockets.begin();
            it != sockets.end(); ++it) {
            bool remove = false;
            AppSocket *appSocket = *it;

            try {
                receive(appSocket->receiveDone() ?
                    (HttpSocket *)appSocket->getApiSocket() :
                    (HttpSocket *)appSocket);

                remove = (appSocket->receiveDone() && appSocket->getApiSocket()->receiveDone());
            }
            catch (std::exception &e) {
                remove = true;
            }

            if (remove) {
                removeSocket(appSocket);
            }
        }
    }


    void receive(HttpSocket *socket) {
        int ret = socket->receive();

        // connection closed or on error
        if (!socket->receiveDone() && ret <= 0 && errno != EAGAIN) {
            throw std::runtime_error("io failed in poll!");
        }
    }

    pthread_mutex_t m_sockets_guard;
    std::set<AppSocket *> m_sockets;
    // strange behavior for tr1::functional;
    NonblockWorker *m_this;
};

#endif