#ifndef CLIENT_BEHAVIOR_H
#define CLIENT_BEHAVIOR_H

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

#endif
