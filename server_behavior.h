#ifndef SERVER_BEHAVIOR_H
#define SERVER_BEHAVIOR_H

#include "networking.h"
#include "calendar.h"

struct SharedCalendar {
    struct DateNode *calendar_head;
    int next_event_id;
    int client_counter;
};

void handle_client_message(int client_socket, int client_id, struct SharedCalendar *shared_cal);
void process_command(char *buffer, int socket, int client_id, struct SharedCalendar *shared_cal);
void handle_create_event(char *buffer, int socket, int client_id, struct SharedCalendar *shared_cal);
void handle_view_events(char *buffer, int socket, int client_id, struct SharedCalendar *shared_cal);
void handle_delete_event(char *buffer, int socket, int client_id, struct SharedCalendar *shared_cal);
void handle_month_view(char *buffer, int socket, int client_id, struct SharedCalendar *shared_cal);
void save_calendar(struct SharedCalendar *shared_cal);
void load_calendar(struct SharedCalendar *shared_cal);
void sighandler(int signo);
struct DateNode* find_or_create_date(int month, int day, int year, struct SharedCalendar *shared_cal);

extern struct SharedCalendar *shared_calendar;
extern int shmid;

#endif