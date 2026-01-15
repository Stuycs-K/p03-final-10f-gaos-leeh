#include <stdio.h>
#include <ncurses.h>
#include "text.h"

// void clear() {
//   printf("\x1b[2J");
// }

void go(int row, int col) {
  printf("\x1b[%d;%df", row, col);
}

void reset() {
  printf("\x1b[0m");
}

void hideCursor() {
  printf("\x1b[?25l");
}

void showCursor() {
  printf("\x1b[?25h");
}

void clear_block(int row, int col, int width, int height) {
  char eraser[width + 1]; eraser[width] = 0;
  for (int i = 0; i < width; i++) {
    eraser[i] = ' ';
  }

  for(int i = 0; i < height; i++) {
    go(row + i,col);
    printf("%s", eraser);
  }
}

  /*Overloaded Colorize methods.
  c1,c2 and c3 are any color modifiers such as bold/color/background color etc.
  */
void printw_color(char* text, int num_mods, int* mods) {
  printw("\x1b[");

  for (int i = 0; i < num_mods; i++) {
    printw("%d", mods[i]);
    if (i != num_mods - 1) printw(";");
  }
  printw("m%s\x1b[0m", text);
}
