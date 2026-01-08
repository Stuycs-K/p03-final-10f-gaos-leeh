#include "networking.h"
#include "client_behavior.h"

int rN() {
  int fd = open("/dev/random", O_RDONLY, 0); if (fd < 0) {printf("%s\n", strerror(errno)); exit(errno);}
  int n1,n2;
  if (read(fd, &n1, sizeof(int)) < 0) {printf("%s\n", strerror(errno)); exit(errno);}
  if (read(fd, &n2, sizeof(int)) < 0) {printf("%s\n", strerror(errno)); exit(errno);}

  int result = n1 * n2; if (result < 0) result *= -1;
  return result;
}

void display_calendar(int month) {
  char* months[] = {"JANUARY", "FEBRUARY", "MARCH", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
  printf("%s\n", months[month - 1]);
}
struct EventNode* create_event(int owner_id, char* name, char* description, int permissions, int* times) {
  struct EventNode* event = (struct EventNode*) malloc(sizeof(struct EventNode));
  event->event_id = rN();

  strcopy(event->name, name);
  strcopy(event->description, description);

  event->owner_id = owner_id;
  event->permissions = permissions;

  if (times) {
    event->all_day = 0;
    event->start_hour = times[0];
    event->start_minute = times[1];
    event->end_hour = times[2];
    event->end_minute = times[3];
  } else {
    event->all_day = 1;
  }

  event->next = NULL;
}
