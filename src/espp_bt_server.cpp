#include <stdio.h>

#include "vamk_socket.h"

#include "vamk_aes.h"
#include "vamk_rfcomm.h"
#include "vamk_rsa.h"
#include "vamk_sdp.h"

#include "espp_bt_client.h"
#include "espp_bt_server.h"

using std::unique_ptr;
using vamk::Sdp;
using vamk::Rsa;
using vamk::RfcommServerSocket;
using vamk::RfcommClientSocket;

namespace espp {

BtServer::BtServer() { _init(); }

BtServer::~BtServer() { stop(); }

void BtServer::start() {
  _init();
  uint8_t uuid[16] = {0x1e, 0x0c, 0xa4, 0xea, 0x29, 0x9d, 0x43, 0x35,
                      0x93, 0xeb, 0x27, 0xfc, 0xfe, 0x7f, 0xa8, 0x48};

  vamk::SdpServiceInfo info = {};
  info.name = "ESPP Music Player";
  info.desc = "Service for music player based on weather from VAMK";
  info.prov = "VAMK";

  printf("Generating RSA key... ");
  fflush(stdout);
  _rsa->generateKeyPair();
  printf("OK.\n");

  printf("Start listening on channel %d. ", CHANNEL);
  _server_socket->listen(CHANNEL);

  printf("Started advertising SDP service.\n\n");
  _sdp->startAdvertise(uuid, CHANNEL, &info);
}

void BtServer::stop() {
  printf("\n");
  if (_sdp) {
    printf("Stopped advertising SDP service. ");
    _sdp->endAdvertise();
  }

  if (_server_socket) {
    printf("Closing server socket.");
    _server_socket->close();
  }

  printf("\n");
  _deinit();
}

std::unique_ptr<BtClient> BtServer::accept() {
  printf("Waiting for client... ");
  fflush(stdout);
  auto client_socket = _server_socket->accept();
  printf("OK.\n");
  return unique_ptr<BtClient>(new BtClient(_rsa, client_socket));
}

// init the member pointer
void BtServer::_init() {
  if (!_sdp)
    _sdp = unique_ptr<Sdp>(new Sdp);
  if (!_rsa)
    _rsa = unique_ptr<Rsa>(new Rsa);
  if (!_server_socket)
    _server_socket = unique_ptr<RfcommServerSocket>(new RfcommServerSocket);
}

// free the member pointer
void BtServer::_deinit() {
  if (_sdp)
    _sdp.reset();
  if (_rsa)
    _rsa.reset();
  if (_server_socket)
    _server_socket.reset();
}
}
