#include "vamk_socket.h"

#include "vamk_rfcomm.h"
#include "vamk_sdp.h"

#define CHANNEL 10

int main() {
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

  int size = client.read();

  printf("Closing client socket.\n");
  client.close();

  printf("Stoped advertising SDP service.\n");
  sdp.endAdvertise();

  printf("Closing server socket.\n");
  server.close();

  return 0;
}
