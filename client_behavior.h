#ifndef CLIENT_BEHAVIOR_H
#define CLIENT_BEHAVIOR_H

#include <ncurses.h>

struct DateNode {
    int month, day, year;
    struct EventNode *events;
    struct DateNode *next;
};

struct EventNode {
    int event_id;
    char name[128];
    char description[256];
    int owner_id;
    int permissions; // -1: private, 0: public
    int all_day; // nonzero : all day ; 0 : time block
    int start_hour, start_minute;
    int end_hour, end_minute;
    struct EventNode *next;
};

void display_calendar(int month);
struct EventNode* create_event(int owner_id, char* name, char* description, int permissions, int* times);

void init_ui();
void cleanup_ui();
void display_main_menu(WINDOW *win);
void display_month_calendar(WINDOW *win, int month, int year);
void create_event_dialog(WINDOW *win, int server_socket, int client_id);
void view_events_dialog(WINDOW *win, int server_socket, int client_id);
int get_days_in_month(int month, int year);

#endif
