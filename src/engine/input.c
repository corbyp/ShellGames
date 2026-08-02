#include "input.h"

#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

struct termios orig_termios;

void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // no echo, no line buffering
    raw.c_cc[VMIN] = 0;   // don't block waiting for a char
    raw.c_cc[VTIME] = 0;  // no timeout either
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int getch(char *c) {
    return read(STDIN_FILENO, c, 1);
}


