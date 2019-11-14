extern "C" {
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
}

#include <sys/socket.h>
#include <unistd.h>

#include "vamk_socket.h"

#include "vamk_rfcomm.h"

namespace vamk {
// server socket
RfcommServerSocket::RfcommServerSocket()
    : Socket(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) {}

void RfcommServerSocket::listen(int channel) {
  // server address struct
  struct sockaddr_rc addr;

  // bind socket to local bluetooth adapter
  bdaddr_t any = {0, 0, 0, 0, 0, 0};
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_bdaddr = any;
  addr.rc_channel = (uint8_t)channel;
  bind(_socket, (struct sockaddr *)&addr, sizeof(addr));

  // put socket into listening mode
  using ::listen;
  listen(_socket, 1);
}

// accept one connection
RfcommClientSocket RfcommServerSocket::accept() {
  // server address struct
  struct sockaddr_rc addr;
  socklen_t opt = sizeof(addr);

  // accept one client
  using ::accept;
  int client = accept(_socket, (struct sockaddr *)&addr, &opt);
  return RfcommClientSocket(client);
}

// client socket
RfcommClientSocket::RfcommClientSocket(int socket) : Socket(socket) {}

int RfcommClientSocket::read(char *buffer, unsigned int size) {
  using ::read;
  return read(_socket, buffer, size);
}

int RfcommClientSocket::write(const char *buffer, unsigned int size) {
  using ::write;
  return write(this->_socket, buffer, size);
}
}
