#include "espp_bt_client.h"
#include "espp_bt_server.h"

#include <stdio.h>

using std::vector;
using espp::BtServer;
using espp::BtClient;

int main() {
  BtServer server;
  server.start();

  auto client = server.accept();

  printf("Exchanging key... ");
  fflush(stdout);
  if (client->exchangeKey())
    printf("OK.\n");
  else
    printf("failed.\n");

  server.stop();

  return 0;
}
