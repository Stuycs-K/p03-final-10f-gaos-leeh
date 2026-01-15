#ifndef UI_H
#define UI_H

#include <ncurses.h>

void init_ui();
void cleanup_ui();
int get_days_in_month(int month, int year);
void add_to_history(const char *cmd);
void get_command_with_history(char *buffer, int max_len);

#endif