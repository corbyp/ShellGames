#include "game.h"

#include <signal.h>

void signalHandler(int sig) {
  (void)sig;
  stop();
}

int main(void) {
  signal(SIGINT, signalHandler);
  start();
  return 0;
}
