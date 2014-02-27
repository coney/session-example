#ifndef __APP_SOCKET_H__
#define __APP_SOCKET_H__

#include "calc.h"
#include "socket-base.h"
#include "api-socket.h"

class AppSocket : public HttpSocket {
public:
    AppSocket(int fd);
    int sendResponse(int value);
    ApiSocket *getApiSocket();

protected:
    bool onReceive(const std::string &content, bool connectionClosed);

    std::pair<int, int> parseAppRequest(const std::string &content);

    std::string generateAppResponse(int value);

private:
    ApiSocket m_apiSocket;
};
#endif