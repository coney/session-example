#ifndef __AIO_WORKER_H__
#define __AIO_WORKER_H__

#include "calc.h"
#include "app-socket.h"

class AIOWorker {
public:
    AIOWorker() {
        m_this = this;
        setupIOCallback();
    }

    int operator()(int fd) {
        m_this->addSocket(fd);
        return 0;
    }

private:
    typedef std::pair<aiocb, HttpSocket *> SocketInfo;

    void setupIOCallback() {
        struct sigaction sig_act;
        sigemptyset(&sig_act.sa_mask);
        sig_act.sa_flags = SA_SIGINFO;
        sig_act.sa_sigaction = sigIOCallback;
        int ret = sigaction( SIGIO, &sig_act, NULL );
        assert(ret == 0);
    }

    static void sigIOCallback(int signo, siginfo_t *info, void *arg) {
        SocketInfo *sockInfo = (SocketInfo*)info->si_value.sival_ptr;
        struct aiocb *cb = &sockInfo->first;
        HttpSocket *socket = sockInfo->second;

        if (aio_error(cb) == 0) {
            int ret = aio_return(cb);
            
            // enclose string with '\0'
            ((char *)cb->aio_buf)[ret] = 0;

            socket->asyncReceive((const char *)cb->aio_buf, ret);
            if (!socket->receiveDone()) {
                // continue reading
                ensure(aio_read(cb) == 0);
            } else {
                delete [] (char *)cb->aio_buf;
                delete sockInfo;

                AppSocket *appSocket = dynamic_cast<AppSocket *>(socket);
                if (appSocket) {
                    // aio only works with a established connection
                    // we need to aio_read after build the connection
                    // add api socket here
                    ((AIOWorker *)NULL)->aioAddSocket(appSocket->getApiSocket());
                }
                else {
                    ApiSocket *apiSocket = dynamic_cast<ApiSocket *>(socket);
                    assert(apiSocket);

                    if (apiSocket->receiveDone() &&
                        apiSocket->getAppSocket()->receiveDone()) {
                            // all complete, clean up
                            delete apiSocket->getAppSocket();
                    }
                }

            }
        }
        else {
            logdebug("error %d errno %d\n", aio_error(cb), errno);
            assert(0);
        }

    }

    void aioAddSocket(HttpSocket *socket) {
        SocketInfo *sockInfo = new SocketInfo(aiocb(), socket);
        memset(&sockInfo->first, 0, sizeof(sockInfo->first));

        sockInfo->first.aio_fildes = socket->getFd();
        sockInfo->first.aio_buf = new char[SOCKET_BUFSIZE];
        sockInfo->first.aio_nbytes = SOCKET_BUFSIZE;

        sockInfo->first.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        sockInfo->first.aio_sigevent.sigev_signo = SIGIO;
        sockInfo->first.aio_sigevent.sigev_value.sival_ptr = sockInfo;

        ensure(aio_read(&sockInfo->first) == 0);
    }

    void addSocket(int fd) {
        AppSocket *socket = new AppSocket(fd);
        aioAddSocket(socket);
    }

    // strange behavior for tr1::functional;
    AIOWorker *m_this;
};

#endif