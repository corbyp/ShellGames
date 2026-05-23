#include "player.h"

extern const int COLS;
extern const int ROWS;

void up(Player *player) {
  if ((*player).y - 1 >= 0)
    --(*player).y;
}

void down(Player *player) {
  if ((*player).y + 1 < ROWS)
    ++(*player).y;
}

void left(Player *player) {
  if ((*player).x - 1 >= 0)
    --(*player).x;
}

void right(Player *player) {
  if ((*player).x + 1 < COLS) {
    ++(*player).x;
  }
}

Player player = {0, 0, up, down, left, right};
