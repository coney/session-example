#ifndef __SOCKET_BASE_H__
#define __SOCKET_BASE_H__
#include <sys/socket.h>
#include <unistd.h>

#include <map>
#include <string>
#include <netinet/in.h>
#include <sstream>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string>
#include <sys/time.h>
#include <fcntl.h>
#define CRLF        "\r\n"

#ifdef SCENE_NONBLOCK
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

        return ::connect(m_fd, (struct sockaddr *)&inaddr, sizeof(inaddr));
    }

    int internalReceive() {
        int ret = 0;
        char m_buffer[4096] = { 0 };

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
#ifdef SCENE_NONBLOCK
        unsigned int start = get_tick_count();
        int ret = internalReceive();
        printf("[%u] receive cost %ums\n", m_fd, get_tick_count() - start);
        return ret;
#else
        return internalReceive();
#endif
    }

    bool receiveDone() {
        return m_recv_done;
    }

    int send(const std::string& content) {
        // assume that there is enough buffer to send the content
        int ret = write(m_fd, content.c_str(), content.size());
        assert(ret == content.size());
        return ret;
    }

    int setNonBlock() {
        int flags = (fcntl(m_fd, F_GETFL, 0) | O_NONBLOCK);
        return fcntl(m_fd, F_SETFL, flags);
    }

    template<class T>
    std::string toString(const T &val) {
        std::string str;
        std::stringstream ss;
        ss << val;
        return ss.str();
    }

protected:
    virtual bool onReceive(const std::string &content, bool connectionClosed) = 0;

private:
    bool m_recv_done;
    int m_pos;
    int m_fd;
    char m_buffer[4096];
};

#endif