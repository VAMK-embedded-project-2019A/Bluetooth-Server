#include "vamk_socket.h"

#include "vamk_rfcomm.h"
#include "vamk_rsa.h"
#include "vamk_sdp.h"

#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#define CHANNEL 10

using std::vector;
using std::cout;
using std::endl;

int main() {
  printf("Generating RSA key ...\n");
  vamk::Rsa rsa;
  rsa.generateKeyPair();

  printf("Initializing bluetooth server...\n");
  vamk::Sdp sdp;
  vamk::RfcommServerSocket server;

  uint8_t uuid[16] = {0x1e, 0x0c, 0xa4, 0xea, 0x29, 0x9d, 0x43, 0x35,
                      0x93, 0xeb, 0x27, 0xfc, 0xfe, 0x7f, 0xa8, 0x48};

  vamk::SdpServiceInfo info = {};
  info.name = "ESPP Music Player";
  info.desc = "Service for music player based on weather from VAMK";
  info.prov = "VAMK";

  printf("Started listening on channel %d.\n", CHANNEL);
  server.listen(CHANNEL);

  printf("Started advertising SDP service.\n");
  sdp.startAdvertise(uuid, CHANNEL, &info);

  printf("Waiting for client...\n");
  auto client = server.accept();
  vector<char> buf;
  int size;

  size = client.read(buf);
  buf.push_back('\0');
  printf("Received %d bytes:\n", size);
  if (size > 0)
    printf("%s\n", &buf[0]);

  vector<char> key = rsa.getPublicKey();
  printf("Sending public key...\n");
  client.write(key);

  size = client.read(buf);
  printf("Received %d bytes.\n", size);
  if (size > 0) {
    auto text = rsa.decrypt(buf);
    printf("Decrypted into %d bytes:\n", text.size());
    text.push_back('\0');
    printf("%s\n", &text[0]);
  }

  printf("Closing client socket.\n");
  client.close();

  printf("Stoped advertising SDP service.\n");
  sdp.endAdvertise();

  printf("Closing server socket.\n");
  server.close();

  return 0;
}
