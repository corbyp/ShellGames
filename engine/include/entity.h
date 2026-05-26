#pragma once

#include <stdbool.h>

#define MAX_ENTITIES 10000

typedef struct Entity {
  int x;
  int y;
  char icon;

  bool visible;
  bool collision;
  bool persistent;

} Entity;

bool move_up(Entity *player, int bound);
bool move_down(Entity *player, int bound);
bool move_left(Entity *player, int bound);
bool move_right(Entity *player, int bound);
bool collide(Entity *entity1, Entity *entity2);
void add_entity(Entity *entity);
Entity get_entity(int index);
int entity_count(void);
