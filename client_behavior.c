#include "networking.h"
#include "client_behavior.h"

void display_calendar(int month) {
  char** months = {"JANUARY", "FEBRUARY", "MARCH", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
  printf("%s\n", months[month - 1]);
}
struct EventNode* create_event(char* name, char* description, int permissions, int* times) {
  struct EventNode* event = (struct EventNode*) malloc(sizeof(struct EventNode));

  strcopy(event->name, name);
  strcopy(event->description, description);

  event->permissions = permissions;

  if (times) {
    event->all_day = 0;
    event->start_hour
  }
}
