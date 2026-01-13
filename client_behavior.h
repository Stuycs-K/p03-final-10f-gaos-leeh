#ifndef CLIENT_BEHAVIOR_H
#define CLIENT_BEHAVIOR_H

#include <ncurses.h>
#include <time.h>
#include "calendar.h"

void display_calendar(struct tm*);
void print_prompt();
struct tm* get_now();
struct EventNode* create_event(int owner_id, char* name, char* description, int permissions, int* times);

void init_ui();
void cleanup_ui();
void display_main_menu(WINDOW *win);
void display_month_calendar(WINDOW *win, int month, int year);
void create_event_dialog(WINDOW *win, int server_socket, int client_id);
void view_events_dialog(WINDOW *win, int server_socket, int client_id);
int get_days_in_month(int month, int year);

#endif
