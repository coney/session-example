#include <stdio.h>
#include <errno.h>

#include "api-socket.h"
#include "app-socket.h"

using namespace std;

bool ApiSocket::onReceive(const std::string &content, bool connectionClosed) {
    //printf("receive [%s] close:%d\n", content.c_str(), connectionClosed);
    if (!connectionClosed) {
        // Should measure the content length, but I'm using
        // the "Connection: Close" in request header
        // Server side will close the connection after replys
        return false;
    }
    m_appSocket->sendResponse(parseResponse(content));
    return true;
}

int ApiSocket::sendRequest(int a, int b) {
    int ret = connect("127.0.0.1", 3000);
    assert(ret == 0);
    string request = generateRequest(a, b);
    ret = send(request);
    assert((unsigned int)ret == request.size());

    return 0;
}

ApiSocket::ApiSocket(AppSocket *appSocket) : HttpSocket() {
    m_appSocket = appSocket;
}

std::string ApiSocket::generateRequest(int a, int b) {
    std::stringstream ss;
    ss << "GET /add?a=" << a << "&b=" << b << " HTTP/1.1" << CRLF
        << "Host: localhost:3000" << CRLF
        << "Connection: close" << CRLF
        << "Accept: text/html" << CRLF
        << "User-Agent: HeHe Client" << CRLF
        << CRLF;
    return ss.str();
}

int ApiSocket::parseResponse(const std::string &response) {
    string::size_type pos = response.find("\r\n\r\n");
    if (pos != string::npos) {
        return atoi(response.substr(pos).c_str());
    }
    return -1;
}

AppSocket * ApiSocket::getAppSocket() {
    return m_appSocket;
}


