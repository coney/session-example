#ifndef __API_SOCKET_H__
#define __API_SOCKET_H__

#include "calc.h"
#include "socket-base.h"

class AppSocket;
class ApiSocket : public HttpSocket{
public:
    ApiSocket(AppSocket *appSocket, bool sync = false);

    // send request, receive and parse response
    int doQuery(int a, int b);

    // just send the request
    int sendRequest(int a, int b);
    AppSocket *getAppSocket();
protected:
    virtual bool onReceive(const std::string &content, bool connectionClosed);

private:
    int parseResponse(const std::string &response);
    std::string generateRequest(int a, int b);
    AppSocket   *m_appSocket;
    bool        m_sync;
    int         m_result;
};

#endif