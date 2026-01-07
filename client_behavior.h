#ifndef CLIENT_BEHAVIOR_H
#define CLIENT_BEHAVIOR_H

void display_calendar(int month);
struct event* create_event(char* name, char* description, int permissions, int all_day);

#endif