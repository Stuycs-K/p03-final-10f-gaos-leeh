#include "client_behavior.h"
#include "networking.h"
#include "text.h"
#include "calendar.h"
#include <ncurses.h>

#define cell_width 24
#define cell_height 8

int main(int argc, char** argv) {
  char* ip = "127.0.0.1";

  if (argc > 1) ip = argv[1];

  int server_socket = client_tcp_handshake(ip);

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  read(server_socket, buffer, BUFFER_SIZE - 1);
  
  time_t raw_now = time(NULL);
  struct tm* now = localtime(&raw_now);
  display_calendar(now);
  
  go(1, 1);
  printf("Connected: %s", buffer);

  while (1) {
        print_prompt();
        fflush(stdout);

        memset(buffer, 0, BUFFER_SIZE);
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        
        if (strncmp(buffer, "quit", 4) == 0) {
            break;
        }

        write(server_socket, buffer, strlen(buffer));

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(server_socket, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            printf("Server disconnected\n");
            break;
        }
        go(terminal_height - 3, 0);
        printf("%s", buffer);
        
        time_t raw_now = time(NULL);
        struct tm* now = localtime(&raw_now);
        display_calendar(now);
    }


    close(server_socket);
    return 0;
}
