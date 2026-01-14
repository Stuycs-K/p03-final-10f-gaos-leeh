#ifndef TEXT_H
#define TEXT_H

#define terminal_width 169
#define terminal_height 45

/*Base colors*/
#define RED 31
#define BLACK 30
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37

/*Text modifiers to be ADDED to a color*/
#define BACKGROUND 10
#define BRIGHT 60

/*Text modifiers that are separate from color*/
#define BOLD 1
#define UNDERLINE 4
#define INVERTED 7

// void clear();
void go(int row, int col);
void reset();
void hideCursor();
void showCursor();
void clear_block(int row, int col, int width, int height);
void printf_color(char* text, int num_mods, int* mods);

#endif
