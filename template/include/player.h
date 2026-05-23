#pragma once
#include <stdint.h>

typedef struct Player {
  uint8_t x;
  uint8_t y;

  void (*up)(struct Player *player);
  void (*down)(struct Player *player);
  void (*left)(struct Player *player);
  void (*right)(struct Player *player);
} Player;

void up(Player *player);
void down(Player *player);
void left(Player *player);
void right(Player *player);

