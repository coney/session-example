#ifndef __CalcServer_H_
#define __CalcServer_H_

#include "calc.h"

class CalcServer {
public:
    typedef std::tr1::function<int(int)> worker_t;
    CalcServer(worker_t worker) {
        m_sockfd = -1;
        m_worker = worker;
    }
    virtual ~CalcServer() {
        close(m_sockfd);
    }


    int run(int port = 8000) {
        m_sockfd = listen(port);
        logdebug("listening at %u\n", port);
        while(1) {
            m_worker(accept());
        }
        return 0;
    }

private:
    int listen(int port){
        int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        assert(fd);

        struct sockaddr_in inaddr = {0};
        inaddr.sin_family = AF_INET;
        inaddr.sin_addr.s_addr = INADDR_ANY;
        inaddr.sin_port = htons(port);

        int flag = 1, ret;
        ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
        assert(ret == 0);

        ret = bind(fd, (struct sockaddr *) &inaddr, sizeof(inaddr));
        assert(ret == 0);

        ret = ::listen(fd, 100);
        assert(ret == 0);

        return fd;
    }

    int accept() {
        int fd;
        struct sockaddr_in inaddr;
        socklen_t inlen = sizeof(inaddr);
        do 
        {
            fd = ::accept(m_sockfd, (struct sockaddr *) &inaddr, &inlen);
        } while (fd == -1 && errno == EINTR);
        if (fd == -1){
            logdebug("accept fd %d\n", fd);
        }

        assert(fd != -1);
        logdebug("connected from %s:%u\n", inet_ntoa(inaddr.sin_addr),
            ntohs(inaddr.sin_port));
        return fd;
    }

    int m_sockfd;
    worker_t m_worker;

};

#endif