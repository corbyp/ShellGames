#include "game.h"
#include "entity.h"
#include "server.h"

#include <stdio.h>
#include <unistd.h>

static double tickrate = 0.15;
static double move_timer = 0.0;
Entity local = {.x = 0, .y = 0, NONE, '@', true, true};
Entity remote = {.x = 0, .y = 0, NONE, 'C', true, true};
Packet packet;

void input_hook(char c) {
  switch (c) {
  case 'w':
    local.direction = UP;
    local.icon = '^';
    break;
  case 'a':
    local.direction = LEFT;
    local.icon = '<';
    break;
  case 's':
    local.direction = DOWN;
    local.icon = 'v';
    break;
  case 'd':
    local.direction = RIGHT;
    local.icon = '>';
    break;
  }
}

void update(Game *game) {
  move_timer += (*game).time_delta;

  if (move_timer >= tickrate) {
    move_timer -= tickrate;

    safe_move(&local);
    if ((*game).server) {
      poll_server(&packet);
    } else if ((*game).client) {
      poll_client(&packet);
    }

    remote.x = packet.x;
    remote.y = packet.y;
    remote.icon = packet.icon;
    packet.x = local.x;
    packet.y = local.y;
    packet.icon = local.icon;

    if ((*game).server) {
      push_server(packet);
    } else if ((*game).client) {
      push_client(packet);
    }
  }
}

void setup(Game *game) {
  (void)game;

  if ((*game).client) {
    printf("Starting remote...\n");
    start_client();
    sleep(1);
  } else if ((*game).server) {
    printf("Starting server...\n");
    start_server();
    sleep(1);
  }

  add_entity(&local);
  add_entity(&remote);
}

void teardown(Game *game) {
  (void)game;
  stop_comms();
}
