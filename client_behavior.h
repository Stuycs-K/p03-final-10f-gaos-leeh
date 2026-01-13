#ifndef CLIENT_BEHAVIOR_H
#define CLIENT_BEHAVIOR_H

#include <time.h>
#include <term.h>
#include "calendar.h"

void display_calendar(struct tm*, int shift);
void print_prompt();
struct tm* get_now();
struct EventNode* create_event(int owner_id, char* name, char* description, int permissions, int* times);

#endif
