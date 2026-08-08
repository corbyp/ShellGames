#include "engine.h"
#include "entity.h"
#include "e2e.h"

#include <stdio.h>
#include <unistd.h>

static double tickrate = 0.15;
static double td_acc = 0.0;
Entity local = {.x = 0,
                .y = 0,
                .direction = DOWN,
                .icon = '@',
                .collision = true,
                .visible = true};
Entity remote = {.x = 0,
                 .y = 0,
                 .direction = NONE,
                 .icon = 'R',
                 .collision = true,
                 .visible = true};
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
  td_acc += (*game).time_delta;
  int recv = -1;
  int pos_changed = 0;

  if ((*game).server) {
    recv = poll_server(&packet);
  } else if ((*game).client) {
    recv = poll_client(&packet);
  }

  if (recv == 0) {
    remote.x = packet.x;
    remote.y = packet.y;
    // remote.icon = packet.icon;
  }

  if (td_acc >= tickrate) {
    td_acc -= tickrate;

    move(&local);
    pos_changed = local.direction != NONE;
  }

  if (pos_changed) {
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
  if ((*game).client) {
    printf("Starting remote...\n");
    start_client((*game).server_ip);
    sleep(1);
  } else if ((*game).server) {
    printf("Starting server...\n");
    start_server();
    sleep(1);
  }

  add_entity(&remote);
  add_entity(&local);
}

void teardown(Game *game) {
  (void)game;
  stop_comms();
}
