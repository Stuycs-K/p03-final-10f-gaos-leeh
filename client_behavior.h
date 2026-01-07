#ifndef CLIENT_BEHAVIOR_H
#define CLIENT_BEHAVIOR_H

struct event {
    char name[128];
    char description[256];
    int permissions; // -1 : private ; 0 : public
    int all_day; // nonzero : all day ; 0 : time block
};

struct date {
    int month, day, year;
    struct event* events;
};

void display_calendar(int month);
struct event* create_event(char* name, char* description, int permissions, int all_day);

#endif