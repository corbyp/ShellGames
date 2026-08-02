#include "game.h"

#include <signal.h>
#include <string.h>
#include <stdio.h>

void signalHandler(int sig) {
  (void)sig;
  stop();
}

int main(int argc, char **argv) {
  signal(SIGINT, signalHandler);
  switch (argc) {
    case 1:
      start(0, 0);
      break;
    case 2:
      if (!strcmp(argv[1], "client")) {
        start(1, 0);
      } else if (!strcmp(argv[1], "server")) {
        start(0, 1);
      } else {
        fprintf(stderr, "Invalid argument\n");
        return -1;
      }
      break;
    default:
      fprintf(stderr, "Invalid number of arguments\n");
      return -1;
  }
  
  return 0;
}
