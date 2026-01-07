#ifndef SERVER_BEHAVIOR_H
#define SERVER_BEHAVIOR_H

#include "networking.h"

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

void handle_client(int client_socket);
void process_command(char *buffer, int socket, int client_id);
void handle_create_event(char *buffer, int socket, int client_id);
void handle_view_events(char *buffer, int socket, int client_id);
void handle_delete_event(char *buffer, int socket, int client_id);
void handle_month_view(char *buffer, int socket, int client_id);
void save_calendar();
void load_calendar();
void sighandler(int signo);
int get_client_id();

extern struct DateNode *calendar_head;
extern int next_event_id;

#endif