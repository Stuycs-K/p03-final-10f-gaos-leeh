#include "networking.h"
#include "client_behavior.h"
#include "text.h"

#define cell_width 24
#define cell_height 8

#define ABS(x) (x < 0)?(x * -1):x

int rN() {
  int fd = open("/dev/random", O_RDONLY, 0); if (fd < 0) {printf("%s\n", strerror(errno)); exit(errno);}
  int n1,n2;
  if (read(fd, &n1, sizeof(int)) < 0) {printf("%s\n", strerror(errno)); exit(errno);}
  if (read(fd, &n2, sizeof(int)) < 0) {printf("%s\n", strerror(errno)); exit(errno);}

  int result = n1 * n2; if (result < 0) result *= -1;
  return result;
}

int weekday(int day, int month, int year) {
  int century = year / 100;
  int shift = year % 100;

  int anchor_lookup[] = {2, 0, 5, 3}; // 2000s: tue 2100s: sun 2200s: friday 2300s: wed
  int doomsdays[] = {(year % 400 && !(year % 4))?4:3, (year % 400 && !(year % 4))?29:28, 14, 4, 9, 6, 11, 8, 5, 10, 7, 12}; // doomsdays by months

  int sum = 0;
  sum += (shift / 12) + (shift % 12) + (shift % 12 % 4) + anchor_lookup[century];
  sum %= 7; // doomsday of tm_year

  int result = (ABS(doomsdays[month] - day) % 7 + sum) % 7;
  return result;
}

void print_frame(int start_row, int start_col) { // figure out resizing terminal
  go(start_row, start_col);

  for (int row = 1; row <= cell_height * 5 + 1; row++) {
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

void display_calendar(struct tm* time, int shift) {
  int display_month = time->tm_mon + shift;

  char* months[] = {"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
  char* week_header = "         Sunday                  Monday                 Tuesday                 Wednesday               Thursday                 Friday                  Saturday      ";

  clear();
  go(1, 1); printf("%s", months[display_month]);
  go(2, 1); printf("%s", week_header);

  int start_weekday = weekday(1, time->tm_mon, time->tm_year);

  print_frame(3, 1);

  int total_days;
  if (display_month == 1) {
    if ((1900 + time->tm_year) % 400 != 0 && (1900 + time->tm_year) % 4 == 0) {
      total_days = 29; // leap year
    } else {
      total_days = 28;
    }
  } else if (display_month < 8) { // before august
    if (display_month % 2) { // even months: apr, jun
      total_days = 30;
    } else { // odd months: jan, mar, may, jul
      total_days = 31;
    }
  } else {
    if (display_month % 2) { // even months: oct, dec
      total_days = 31;
    } else { // odd months: sep, nov
      total_days = 30;
    }
  }

  int curr_row = 4;
  int cell_margin = 3;
  for(int i = 0; i < total_days; i++) {
    go(curr_row, start_weekday * cell_width + cell_margin);
    if (i + 1 == time->tm_mon) {
      char text[11];
      int mods[] = {WHITE + BACKGROUND};
      err(sprintf(text, "%d", i), "sprintf failed");
      printf_color(text, 2, mods);
    } else {
      printf("%d", i + 1);
    }
    start_weekday++;

    if (start_weekday == 7) {
      curr_row += cell_height;
      start_weekday %= 7;
    }
  }
}

void print_prompt() {
  go(terminal_height, 0); printf("insert valid commands here");
  go(terminal_height - 1, 0); printf("enter command: ");
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
