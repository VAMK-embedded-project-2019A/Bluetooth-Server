#ifndef VAMK_SOCKET
#define VAMK_SOCKET

namespace vamk {

class Socket {
public:
    Socket(int socket);
    void close();
protected:
    int _socket;
};
}

#endif