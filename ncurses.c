#include "networking.h"
#include "client_behavior.h"
#include <ncurses.h>
#include "text.h"

#define MAX_HISTORY 50
#define MAX_CMD_LEN 256

static char command_history[MAX_HISTORY][MAX_CMD_LEN];
static int history_count = 0;
static int history_index = 0;

void init_ui() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    }
}

void cleanup_ui() {
    endwin();
}

void add_to_history(const char *cmd) {
    if (strlen(cmd) == 0) return;
    
    strncpy(command_history[history_count % MAX_HISTORY], cmd, MAX_CMD_LEN - 1);
    history_count++;
    history_index = history_count;
}

void get_command_with_history(char *buffer, int max_len) {
    int pos = 0;
    int ch;
    
    buffer[0] = '\0';
    
    while (1) {
        ch = getch();
        
        if (ch == '\n') {
            buffer[pos] = '\0';
            break;
        }
        else if (ch == KEY_UP) {
            if (history_index > 0 && history_count > 0) {
                history_index--;
                int actual_index = history_index % MAX_HISTORY;
                if (actual_index < history_count) {
                    strcpy(buffer, command_history[actual_index]);
                    pos = strlen(buffer);
                    
                    move(terminal_height - 1, 15);
                    clrtoeol();
                    printw("%s", buffer);
                    refresh();
                }
            }
        }
        else if (ch == KEY_DOWN) {
            if (history_index < history_count - 1) {
                history_index++;
                int actual_index = history_index % MAX_HISTORY;
                strcpy(buffer, command_history[actual_index]);
                pos = strlen(buffer);
                
                move(terminal_height - 1, 15);
                clrtoeol();
                printw("%s", buffer);
                refresh();
            } else if (history_index == history_count - 1) {
                history_index = history_count;
                buffer[0] = '\0';
                pos = 0;
                
                move(terminal_height - 1, 15);
                clrtoeol();
                refresh();
            }
        }
        else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (pos > 0) {
                pos--;
                buffer[pos] = '\0';
                
                int y, x;
                getyx(stdscr, y, x);
                mvaddch(y, x - 1, ' ');
                move(y, x - 1);
                refresh();
            }
        }
        else if (ch >= 32 && ch < 127 && pos < max_len - 1) {
            buffer[pos++] = ch;
            buffer[pos] = '\0';
            addch(ch);
            refresh();
        }
    }
    
    buffer[pos] = '\0';
}

int get_days_in_month(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        return 29;
    }
    
    return days[month - 1];
}