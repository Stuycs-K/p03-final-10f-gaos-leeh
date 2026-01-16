#ifndef CLIENT_BEHAVIOR_H
#define CLIENT_BEHAVIOR_H

#include <time.h>
#include "calendar.h"
#include "ui.h"

void display_calendar(struct tm* now, int shift);
void print_prompt();
struct tm* get_now();
struct EventNode* create_event(int owner_id, char* name, char* description, int permissions, int* times);

#endif
