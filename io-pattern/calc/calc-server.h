#ifndef __CalcServer_H_
#define __CalcServer_H_

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <assert.h>

#include <functional>
#include <boost/tr1/functional.hpp>

class CalcServer {
    typedef std::tr1::function<int(int)> worker_t;
public:
    CalcServer(worker_t worker) {
        m_sockfd = -1;
        m_worker = worker;
    }
    virtual ~CalcServer() {
        close(m_sockfd);
    }
    int run(int port = 8000) {
        m_sockfd = listen(port);
        printf("listening at %u\n", port);
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



        ret = ::listen(fd, 5);
        assert(ret == 0);

        return fd;
    }

    int accept() {
        int fd;
        struct sockaddr_in inaddr;
        socklen_t inlen;
        fd = ::accept(m_sockfd, (struct sockaddr *) &inaddr, &inlen);
        assert(fd != -1);
        printf("connected from %s:%u\n", inet_ntoa(inaddr.sin_addr),
                ntohs(inaddr.sin_port));
        return fd;
    }

    int m_sockfd;
    worker_t m_worker;

};

#endif