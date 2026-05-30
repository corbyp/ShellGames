// input.c
#include "input.h"
#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>

extern void input_hook(int keys[]);

#define KEYBOARD "/dev/input/event23" // adjust this for your keyboard

void *input_loop(void *_) {
  int fd = open(KEYBOARD, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return NULL;
  }

  struct input_event ev;
  int keys[KEY_MAX] = {0}; // 1 = held, 0 = released

  while (1) {
    read(fd, &ev, sizeof(ev));

    if (ev.type == EV_KEY) {
      if (ev.value == 1)
        keys[ev.code] = 1; // key down
      else if (ev.value == 0)
        keys[ev.code] = 0; // key up
      // ev.value == 2 is key repeat, ignore for game input

      // Example: check WASD
      input_hook(keys);
      // printf("W:%d A:%d S:%d D:%d\n", keys[KEY_W], keys[KEY_A], keys[KEY_S],
      //        keys[KEY_D]);
    }
  }

  close(fd);
  return NULL;
}

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

// void *input_loop(void *_) {
//   (void)_;
//   raw();
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
