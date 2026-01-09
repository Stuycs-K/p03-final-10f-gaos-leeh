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

char* repeat(char* s, int length, int reps) {
  char*  result = (char *) malloc(sizeof(char) length * reps + 1);
  char* temp = result;

  for (int i = 0; i < length * reps + 1; i++) {
    strcpy(temp, s);
    temp += length;
  }

  *temp = 0;
  return result;
}

void print_frame(int width, int height, int start_day) { // figure out resizing terminal
  int cell_width = width / 7;
  int cell_height = height / 4;

  for (int i = 1; i < height; i++) {
    printf("\u001b[%d;%df", i, 0);  // leftmost wall
    printf("\u001b[%d;%df|", i, width - 1); // rightmost wall
  }

  char* bar = repeat("-", 1, width - 2);

  printf("\u001b[%d;%df+%s+", 1, 0, bar);
  printf("\u001b[%d;%df+%s+", height - 1, 0, bar);

  free(bar);
}

void display_calendar(int month) {
  char* months[] = {"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
  printf("%s\n", months[month - 1]);

  print_frame(80, 24, 0);
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
