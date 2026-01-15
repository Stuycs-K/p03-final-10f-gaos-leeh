#ifndef CALENDAR_H
#define CALENDAR_H

#define shift 0

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
#endif