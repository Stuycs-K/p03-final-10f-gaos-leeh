#include "client_behavior.h"
#include "networking.h"

int main(int argc, char** argv) {
  char* ip = "127.0.0.1";

  if (argc > 1) ip = argv[1];

  int server_socket = client_tcp_handshake(ip);
  printf("client connected.\n");
}
