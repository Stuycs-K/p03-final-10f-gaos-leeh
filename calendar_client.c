#include "client_behavior.h"
#include "networking.h"
#include "calendar.h"
#include <ncurses.h>

void show_help() {
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(0, 0, "CALENDAR COMMAND REFERENCE");
    attroff(COLOR_PAIR(1) | A_BOLD);
    
    mvprintw(2, 0, "NAVIGATION:");
    mvprintw(3, 2, "Left/Right Arrow  - Previous/Next month");
    mvprintw(4, 2, "Up/Down Arrow     - Move 3 months back/forward");
    mvprintw(5, 2, "Up/Down (command) - Navigate command history");
    
    mvprintw(7, 0, "COMMANDS:");
    mvprintw(8, 2, "CREATE name|description|permissions|all_day|month|day|year");
    mvprintw(9, 4, "permissions: 0=public, -1=private");
    mvprintw(10, 4, "all_day: 1=all day, 0=time block");
    mvprintw(11, 4, "For timed events, add: |start_hour|start_min|end_hour|end_min");
    mvprintw(12, 4, "Example: CREATE Meeting|Discuss project|0|1|1|15|2026");
    
    mvprintw(14, 2, "VIEW month|day|year");
    mvprintw(15, 4, "Example: VIEW 1|15|2026");
    
    mvprintw(17, 2, "MONTH month|year");
    mvprintw(18, 4, "Example: MONTH 1|2026");
    
    mvprintw(20, 2, "DELETE event_id");
    mvprintw(21, 4, "Example: DELETE 1");
    
    mvprintw(23, 2, "HELP - Show this help screen");
    mvprintw(24, 2, "quit - Exit the calendar");
    
    mvprintw(LINES - 1, 0, "Press any key to return...");
    refresh();
    getch();
}

int main(int argc, char** argv) {
  char* ip = "127.0.0.1";

  if (argc > 1) ip = argv[1];

  int server_socket = client_tcp_handshake(ip);

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  int bytes = read(server_socket, buffer, BUFFER_SIZE - 1);
  if (bytes > 0) buffer[bytes] = '\0';

  init_ui();

  mvprintw(0, 0, "Connected: %s", buffer);
  mvprintw(1, 0, "Type 'HELP' for command reference");
  refresh();
  getch();

  time_t raw_now = time(NULL);
  struct tm* now = localtime(&raw_now);
  int current_month = now->tm_mon;
  int current_year = now->tm_year + 1900;

  int shift = 0;
  while (1) {
        clear();

        struct tm display_time = {0};
        display_time.tm_mon = current_month;
        display_time.tm_year = current_year - 1900;
        display_time.tm_mday = 1;
        mktime(&display_time);

        display_calendar(now, shift);

        mvprintw(LINES - 4, 0, "Arrow Keys: Navigate months | Commands: CREATE, VIEW, MONTH, DELETE");
        mvprintw(LINES - 3, 0, "Current view: Month %d/%d", current_month + 1, current_year);
        mvprintw(LINES - 2, 0, "Type 'quit' to exit");
        mvprintw(LINES - 1, 0, "Enter command: ");
        refresh();

        char command[BUFFER_SIZE];
        memset(command, 0, BUFFER_SIZE);

        int ch = getch();

        if (ch == KEY_LEFT) {
            current_month--;
            if (current_month < 0) {
                current_month = 11;
                current_year--;
                if (current_year < 2026) current_year = 2026;
            }
            shift--;
            continue;
        } else if (ch == KEY_RIGHT) {
            current_month++;
            if (current_month > 11) {
                current_month = 0;
                current_year++;
                if (current_year > 2026) current_year = 2026;
            }
            shift++;
            continue;
        } else if (ch == KEY_UP) {
            current_month -= 3;
            while (current_month < 0) {
                current_month += 12;
                current_year--;
                if (current_year < 2026) {
                    current_year = 2026;
                    current_month = 0;
                    break;
                }
            }
            continue;
        } else if (ch == KEY_DOWN) {
            current_month += 3;
            while (current_month > 11) {
                current_month -= 12;
                current_year++;
                if (current_year > 2026) {
                    current_year = 2026;
                    current_month = 11;
                    break;
                }
            }
            continue;
        } else {
            ungetch(ch);
        }

        get_command_with_history(command, BUFFER_SIZE);

        if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
            break;
        }
        if (strcmp(command, "HELP") == 0 || strcmp(command, "help") == 0) {
            show_help();
            continue;
        }

        if (strlen(command) == 0) continue;

        add_to_history(command);

        strcat(command, "\n");
        write(server_socket, command, strlen(command));

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(server_socket, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            mvprintw(LINES/2, 0, "Server disconnected");
            refresh();
            getch();
            break;
        }
        buffer[bytes_read] = '\0';

        clear();
        mvprintw(0, 0, "Server Response:");
        mvprintw(2, 0, "%s", buffer);
        mvprintw(LINES - 1, 0, "Press any key to continue...");
        refresh();
        getch();
    }

    cleanup_ui();
    close(server_socket);
    return 0;
}
