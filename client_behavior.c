#include "networking.h"
#include "client_behavior.h"
#include "text.h"

#define cell_width 24
#define cell_height 8

int rN() {
  int fd = open("/dev/random", O_RDONLY, 0); if (fd < 0) {printw("%s\n", strerror(errno)); exit(errno);}
  int n1,n2;
  if (read(fd, &n1, sizeof(int)) < 0) {printw("%s\n", strerror(errno)); exit(errno);}
  if (read(fd, &n2, sizeof(int)) < 0) {printw("%s\n", strerror(errno)); exit(errno);}

  int result = n1 * n2; if (result < 0) result *= -1;
  return result;
}
void print_frame(int start_row, int start_col) {
  for (int row = 0; row <= cell_height * 6; row++) {
    for (int col = 0; col <= cell_width * 7; col++) {
        move(start_row + row, start_col + col);
        if (col % cell_width == 0 && row % cell_height == 0) {
            printw("+");
        } else if (col % cell_width == 0) {
            printw("|");
        } else if (row % cell_height == 0) {
            printw("-");
        }
    }
  }
}

void display_calendar(struct tm* time, int shift) {
  char* months[] = {"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", 
                    "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
  char* week_header = "         Sunday                  Monday                 Tuesday                 Wednesday               Thursday                 Friday                  Saturday      ";

  // Calculate the display month/year based on shift
  int display_month = time->tm_mon + shift;
  int display_year = 1900 + time->tm_year;
  
  while (display_month < 0) {
    display_month += 12;
    display_year--;
  }
  while (display_month > 11) {
    display_month -= 12;
    display_year++;
  }

  attron(COLOR_PAIR(1) | A_BOLD);
  mvprintw(0, 0, "%s %d", months[display_month], display_year);
  attroff(COLOR_PAIR(1) | A_BOLD);
  
  mvprintw(1, 0, "%s", week_header);

  // Create a tm struct for the first day of the display month
  struct tm first_day = {0};
  first_day.tm_year = display_year - 1900;
  first_day.tm_mon = display_month;
  first_day.tm_mday = 1;
  mktime(&first_day);

  int start_weekday = first_day.tm_wday;

  print_frame(2, 0);

  // Calculate days in month
  int total_days;
  if (display_month == 1) {  // February
    if ((display_year % 400 == 0) || (display_year % 4 == 0 && display_year % 100 != 0)) {
      total_days = 29;
    } else {
      total_days = 28;
    }
  } else if (display_month == 3 || display_month == 5 || display_month == 8 || display_month == 10) {
    total_days = 30;  // April, June, September, November
  } else {
    total_days = 31;  // Jan, March, May, July, Aug, Oct, Dec
  }

  int curr_row = 3;
  int cell_margin = 2;
  
  for(int day = 1; day <= total_days; day++) {
    // Highlight today's date if we're viewing the current month
    if (day == time->tm_mday && 
        display_month == time->tm_mon && 
        display_year == (1900 + time->tm_year)) {
      attron(COLOR_PAIR(2) | A_BOLD);
      mvprintw(curr_row, start_weekday * cell_width + cell_margin, "%d", day);
      attroff(COLOR_PAIR(2) | A_BOLD);
    } else {
      mvprintw(curr_row, start_weekday * cell_width + cell_margin, "%d", day);
    }
    
    start_weekday++;
    if (start_weekday == 7) {
      curr_row += cell_height;
      start_weekday = 0;
    }
  }
  
  refresh();
}

void print_prompt() {
  move(terminal_height, 0); printw("insert valid commands here");
  move(terminal_height - 1, 0); printw("enter command: ");
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
