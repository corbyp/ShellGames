// input.c
// #include "input.h"
// #include "game.h"

// #include <stdbool.h>
// #include <stdio.h>
// #include <termios.h>
// #include <unistd.h>

// #include <fcntl.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <linux/input.h>

// extern void input_hook(char c);

// char getch() {
//   char buf = 0;
//   struct termios old = {0};
//   if (tcgetattr(0, &old) < 0)
//     perror("tcsetattr()");
//   old.c_lflag &= ~ICANON;
//   old.c_lflag &= ~ECHO;
//   old.c_cc[VMIN] = 1;
//   old.c_cc[VTIME] = 0;
//   if (tcsetattr(0, TCSANOW, &old) < 0)
//     perror("tcsetattr ICANON");
//   if (read(0, &buf, 1) < 0)
//     perror("read()");
//   old.c_lflag |= ICANON;
//   old.c_lflag |= ECHO;
//   if (tcsetattr(0, TCSADRAIN, &old) < 0)
//     perror("tcsetattr ~ICANON");
//   return (buf);
// }

// void *input_loop(void *_) {
//   (void)_;
//   // raw();
//   char c;

//   while (true) {
//     c = getch();
//     switch (c) {
//     case 'q':
//       stop();
//       break;
//     case 'v':
//       toggle_verbose();
//       break;
//     }
//     input_hook(c);
//   }
// }
#include "input.h"

#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

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


