#ifndef __SOCKET_BASE_H__
#define __SOCKET_BASE_H__

#include "calc.h"

#define CRLF        "\r\n"

#ifdef LOCAL_DEBUG
static inline unsigned int get_tick_count() {
    struct timeval ts;
    gettimeofday(&ts, NULL);
    return ts.tv_sec * 1000 + ts.tv_usec / 1000;
}
#endif

class HttpSocket {
public:
    HttpSocket(int fd = -1) {
        // if no fd specified, create one
        if (fd == -1) {
            fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            assert(fd != -1);
        }
        m_fd = fd;
        m_pos = 0;
        m_recv_done = false;
        memset(m_buffer, 0, sizeof(m_buffer));
    }

    virtual ~HttpSocket() {
        close(m_fd);
    }

    int getFd() {
        return m_fd;
    }

    int connect(const std::string& ip, unsigned short hport) {
        struct sockaddr_in inaddr = { 0 };
        inaddr.sin_family = AF_INET;
        inaddr.sin_addr.s_addr = inet_addr(ip.c_str());
        inaddr.sin_port = htons(hport);

        int ret = ::connect(m_fd, (struct sockaddr *)&inaddr, sizeof(inaddr));
        if (ret < 0 && errno == EINPROGRESS) {
            // non-block socket, wait for connection established
            fd_set wfds;
            FD_ZERO(&wfds);
            FD_SET(m_fd, &wfds);

            struct timeval timeout = {5};
            ret = select(m_fd + 1, NULL, &wfds, NULL, &timeout);
            assert(ret == 1);
            return 0;
        }
        return ret;
    }

    int internalReceive() {
        int ret = 0;

        while (m_pos < (sizeof(m_buffer)-1) && (ret = read(m_fd, m_buffer + m_pos, sizeof(m_buffer)-m_pos)) >= 0) {
            m_pos += ret;
            if (onReceive(m_buffer, ret == 0)) {
                m_recv_done = true;
                m_pos = 0;
                memset(m_buffer, 0, sizeof(m_buffer));
                break;
            }
        }

        return ret;
    }

    int receive() {
#ifdef LOCAL_DEBUG
        unsigned int start = get_tick_count();
        int ret = internalReceive();
        logdebug("fd [%u] receive cost %ums\n", m_fd, get_tick_count() - start);
        return ret;
#else
        return internalReceive();
#endif
    }

    int receive(const char *buf, int len) {
        assert(sizeof(m_buffer) - m_pos > (unsigned int)len);
        if (len) {
            memcpy(m_buffer + m_pos, buf, len);
            m_pos += len;
        }

        if (onReceive(m_buffer, len == 0)) {
            m_recv_done = true;
            m_pos = 0;
            memset(m_buffer, 0, sizeof(m_buffer));
        }
        return len;
    }

    bool receiveDone() {
        return m_recv_done;
    }

    int send(const std::string& content) {
        // assume that there is enough buffer to send the content
        int ret = write(m_fd, content.c_str(), content.size());
        assert((unsigned int)ret == content.size());
        return ret;
    }

    int setNonBlock() {
        int flags = (fcntl(m_fd, F_GETFL, 0) | O_NONBLOCK);
        return fcntl(m_fd, F_SETFL, flags);
    }

protected:
    virtual bool onReceive(const std::string &content, bool connectionClosed) = 0;

private:
    bool m_recv_done;
    unsigned int m_pos;
    int m_fd;
    char m_buffer[4096];
};

#endif
