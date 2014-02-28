#include "app-socket.h"

using namespace std;

std::string AppSocket::generateAppResponse(int value)
{
    stringstream ss;
    ss << "HTTP/1.1 200 OK" << CRLF
        << "Content-Type: text/plain" << CRLF
        << "HeHe Server" << CRLF << CRLF
        << value;
    return ss.str();
}

std::pair<int, int> AppSocket::parseAppRequest(const std::string &content)
{
    int a, b;
    if (sscanf(content.c_str(), "GET /%d+%d", &a, &b) != 2) {
        throw std::invalid_argument(content);
    }
    return std::make_pair(a, b);
}

bool AppSocket::onReceive(const std::string &content, bool connectionClosed)
{
    try {
        std::pair<int, int> values = parseAppRequest(content);
        if (m_sync) {
            sendResponse(m_apiSocket.doQuery(values.first, values.second));
        }
        else {
            m_apiSocket.sendRequest(values.first, values.second);
        }
    }
    catch (std::exception &e) {
        logdebug("AppSocket exception: %s\n", e.what());
        return false;
    }

    return true;
}

int AppSocket::sendResponse(int value)
{
    return send(generateAppResponse(value));
}

AppSocket::AppSocket(int fd, bool sync /*= false*/ ) 
    : HttpSocket(fd), m_apiSocket(this, sync), m_sync(sync)
{

}

ApiSocket * AppSocket::getApiSocket()
{
    return &m_apiSocket;
}
