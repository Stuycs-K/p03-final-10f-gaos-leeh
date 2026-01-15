#ifndef CLIENT_BEHAVIOR_H
#define CLIENT_BEHAVIOR_H

#include <time.h>
#include "calendar.h"

void display_calendar(struct tm*, int shift);
void print_prompt();
struct tm* get_now();
struct EventNode* create_event(int owner_id, char* name, char* description, int permissions, int* times);

void init_ui();
void cleanup_ui();
int get_days_in_month(int month, int year);
void add_to_history(const char *cmd);
void get_command_with_history(char *buffer, int max_len);

#endif
