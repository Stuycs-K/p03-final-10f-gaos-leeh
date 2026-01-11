#include "networking.h"
#include "client_behavior.h"
#include "text.h"

int rN() {
  int fd = open("/dev/random", O_RDONLY, 0); if (fd < 0) {printf("%s\n", strerror(errno)); exit(errno);}
  int n1,n2;
  if (read(fd, &n1, sizeof(int)) < 0) {printf("%s\n", strerror(errno)); exit(errno);}
  if (read(fd, &n2, sizeof(int)) < 0) {printf("%s\n", strerror(errno)); exit(errno);}

  int result = n1 * n2; if (result < 0) result *= -1;
  return result;
}

void print_frame(int width, int height, int start_day) { // figure out resizing terminal
  int cell_width = width / 7;
  int cell_height = height / 4;

  for (int row = 0; row <= height; row++) {
    for (int col = 0; col <= width; col++) {
        if (col % cell_width == 0 && (row - 1) % cell_height == 0) {
            printf("+");
        } else if (col % cell_width == 0) {
            printf("|");
        } else if (row % cell_height == 0) {
            printf("-");
        } else {
            printf(" ");
        }
    }
  }
}

void display_calendar(int month) {
  char* months[] = {"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};

  clear();
  go(0,0); // printf("%s", months[month - 1]);

  print_frame(84, 24, 0);
}
struct EventNode* create_event(int owner_id, char* name, char* description, int permissions, int* times) {
  struct EventNode* event = (struct EventNode*) malloc(sizeof(struct EventNode));
  event->event_id = rN();

  strcpy(event->name, name);
  strcpy(event->description, description);

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

  return event;
}
