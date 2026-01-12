#include "networking.h"
#include "client_behavior.h"

void init_ui() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    }
}

void cleanup_ui() {
    endwin();
}

void display_main_menu(WINDOW *win) {
    werase(win);
    box(win, 0, 0);
    
    wattron(win, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(win, 1, 2, "SHARED CALENDAR - MAIN MENU");
    wattroff(win, COLOR_PAIR(1) | A_BOLD);
    
    mvwprintw(win, 3, 2, "1. View Month Calendar");
    mvwprintw(win, 4, 2, "2. Create Event");
    mvwprintw(win, 5, 2, "3. View Events on Date");
    mvwprintw(win, 6, 2, "4. Delete Event");
    mvwprintw(win, 7, 2, "5. Quit");
    
    mvwprintw(win, 9, 2, "Choice: ");
    
    wrefresh(win);
}

int get_days_in_month(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        return 29;
    }
    
    return days[month - 1];
}