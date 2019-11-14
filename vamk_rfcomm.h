#ifndef VAMK_RFCOMM
#define VAMK_RFCOMM

namespace vamk {
class Socket;
class RfcommClientSocket;

class RfcommServerSocket : public Socket {
public:
  // delete implicit constructor
  RfcommServerSocket(const RfcommServerSocket &) = delete;
  RfcommServerSocket operator=(const RfcommServerSocket &) = delete;

  // allocate the socket
  RfcommServerSocket();

  // bind, start listening
  void listen(int channel);

  // accept a client
  RfcommClientSocket accept();

private:
};

class RfcommClientSocket : public Socket {
public:
  // delete implicit constructor
  RfcommClientSocket() = delete;
  RfcommServerSocket operator=(const RfcommClientSocket &) = delete;

  // read from stream
  int read(char *, unsigned int);

  // write to stream
  int write(const char *, unsigned int);

private:
  // allocate the socket
  RfcommClientSocket(int socket);

  // only server can construct client
  friend RfcommServerSocket;
};
}

#endif
