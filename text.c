#include <stdio.h>

void clear() {
    printf("\x1b[2J");
}

void go(int row, int col) {
    printf("\x1b[%d;%df", row, col);
}
