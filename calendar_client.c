#include "client_behavior.h"
#include "networking.h"

int main(int argc, char** argv) {
  char* ip = "127.0.0.1";

  if (argc > 1) ip = argv[1];

  int server_socket = client_tcp_handshake(ip);
  printf("client connected.\n");

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  read(server_socket, buffer, BUFFER_SIZE - 1);
  printf("%s", buffer);

  while (1) {
        printf("\nenter command: ");
        fflush(stdout);
        
        memset(buffer, 0, BUFFER_SIZE);
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        
        write(server_socket, buffer, strlen(buffer));
        
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(server_socket, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            printf("Server disconnected\n");
            break;
        }
        
        printf("%s", buffer);
    }
    
    close(server_socket);
    return 0;
}
