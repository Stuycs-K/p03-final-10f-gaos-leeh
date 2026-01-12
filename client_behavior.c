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

void print_frame(int start_row, int start_col) { // figure out resizing terminal
  go(start_row, start_col);

  int cell_width = 24;
  int cell_height = 10;

  for (int row = 1; row <= cell_height * 4 + 1; row++) {
    for (int col = 1; col <= cell_width * 7 + 1; col++) {
        go(start_row + row - 1, start_col + col - 1);
        if ((col - 1) % cell_width == 0 && (row - 1) % cell_height == 0) {
            printf("+");
        } else if ((col - 1) % cell_width == 0) {
            printf("|");
        } else if ((row - 1) % cell_height == 0) {
            printf("-");
        }
    }
  }
}

void display_calendar(int month) {
  char* months[] = {"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
  char* week_header = "         Sunday        ";

  clear();
  go(1, 1); printf("%s", months[month - 1]);
  go(2, 1); printf("%s", week_header);

  print_frame(3, 1);
}

void print_prompt() {
  go(45, 0); printf("insert valid commands here");
  go(44, 0); printf("enter command: ");
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
