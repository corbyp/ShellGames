#include "engine.h"

#include <signal.h>
#include <string.h>
#include <stdio.h>

void signalHandler(int sig) {
  (void)sig;
  stop();
}

int main(int argc, char **argv) {
  signal(SIGINT, signalHandler);
  Game game = {
    .time_delta = 0,
    .timer = 0,
    .rows = 0,
    .cols = 0,
    .client = 0,
    .server = 0,
    .server_ip = "",

    .background = ' ',
    
    .bottom = ' ',
    .bottom_left = ' ',
    .bottom_right = ' ',

    .top = ' ',
    .top_left = ' ',
    .top_right = ' ',

    .left = ' ',
    .right = ' ',
  };

  switch (argc) {
    case 1:
      start(game);
      break;
    case 2:
      if (!strcmp(argv[1], "client")) {
        game.client = 1;
        game.server_ip = "127.0.0.1";
        start(game);
      } else if (!strcmp(argv[1], "server")) {
        game.server = 1;
        start(game);
      } else {
        fprintf(stderr, "Invalid argument\n");
        return -1;
      }
      break;
    case 3:
      if (!strcmp(argv[1], "client")) {
        game.client = 1;
        game.server_ip = argv[2];
        start(game);
      } else {
        fprintf(stderr, "Not calling client with ip addr");
        return -1;
      }
      break;
    default:
      fprintf(stderr, "Invalid number of arguments\n");
      return -1;
  }
  
  return 0;
}
