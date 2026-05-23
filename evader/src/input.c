#include "input.h"

#include <stdbool.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

extern bool running;
extern bool verbose;
extern void input_hook(char c);

char getch() {
  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
    perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("tcsetattr ~ICANON");
  return (buf);
}

void *input_loop(void *_) {
  char cur;

  while (running) {
    cur = getch();

    switch (cur) {
      case 'q':
        running = false;
        break;
      case 'v':
        verbose = !verbose;
        break;
    }

    input_hook(cur);
  }

  return NULL;
}
