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
  printf("client connected.\n");

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  read(server_socket, buffer, BUFFER_SIZE - 1);
  printf("%s", buffer);

  sleep(1);
  init_ui();

  time_t raw_now = time(NULL);
  struct tm* now = localtime(&raw_now);

  int shift = 0;
  display_calendar(now, shift);

  while (1) {
        print_prompt();
        fflush(stdout);

        move(terminal_height - 2, 0);
        printw("Commands: CREATE, VIEW, MONTH, DELETE (or 'quit' to exit)");
        move(terminal_height - 1, 0);
        printw("enter command: ");
        refresh();

        char command[BUFFER_SIZE];
        get_command_with_history(command, BUFFER_SIZE);

        if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
            break;
        }

        add_to_history(command);

        strcat(command, "\n");
        write(server_socket, command, strlen(command));

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(server_socket, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            mvprintw(terminal_height - 3, 0, "Server disconnected");
            refresh();
            getch();
            break;
        }

        mvprintw(0, 0, "Server Response:");
        mvprintw(1, 0, "%s", buffer);
        mvprintw(terminal_height - 1, 0, "Press any key to continue...");
        refresh();
        getch();
    }

    cleanup_ui();
    close(server_socket);
    return 0;
}
