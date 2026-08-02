#include "engine.h"
#include "entity.h"

#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>

static Entity player = {0, 0, NONE, '@', true, true};
Game *game2;

bool in_air(Game *game) { return player.y < (*game).rows - 1; }
bool on_ground(Game *game) { return player.y == (*game).rows - 1; }
bool zenith(Game *game) { return player.y < (*game).rows - 4; }
bool jumping = false;
bool falling = true;

void input_hook(char c) {
  // if (keys[KEY_W]) {
  //   player.direction = UP;
  // }
  // if (keys[KEY_A]) {
  //   player.direction = LEFT;
  // }
  // if (keys[KEY_S]) {
  //   player.direction = DOWN;
  // }
  // if (keys[KEY_D]) {
  //   player.direction = RIGHT;
  // }
  // safe_move(&player);
}

void update(Game *game) {
  if ((*game).loop_delta % 5 == 0) {
    if (keys2[0]) {
      player.direction = UP;
      safe_move(&player);
    }
    if (keys2[1]) {
      player.direction = LEFT;
      safe_move(&player);
    }
    if (keys2[2]) {
      player.direction = DOWN;
      safe_move(&player);
    }
    if (keys2[3]) {
      player.direction = RIGHT;
      safe_move(&player);
    }

    player.direction = NONE;
  }
  printf("W:%d A:%d S:%d D:%d\n", keys2[0], keys2[1], keys2[2], keys2[3]);
}

void setup(Game *game) {
  player.x = (*game).cols / 2;
  player.y = (*game).rows - 1;
  add_entity(&player);
}

void teardown(Game *game) {}
