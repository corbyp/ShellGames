#include "game.h"
#include "player.h"

#include <stdio.h>

extern Player player;
const int ROWS = 8;
const int COLS = 2 * ROWS;
const char PLAYER_CHAR = 'x';

void input_hook(char c) {
  switch (c) {
  case 'w':
    player.up(&player);
    break;
  case 's':
    player.down(&player);
    break;
  case 'a':
    player.left(&player);
    break;
  case 'd':
    player.right(&player);
    break;
  }
}

void update(char arr[ROWS][COLS + 1]) {
  printf("x: %d, y: %d\n", player.x, player.y);
  arr[player.y][player.x] = PLAYER_CHAR;
}

int main(void) {
  start_game();
  return 0;
}
