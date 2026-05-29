#pragma once

#include <stdbool.h>

#define MAX_ENTITIES 10000

typedef enum Direction {
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
} Direction;

typedef struct Entity {
  int x;
  int y;
  Direction direction;
  char icon;

  bool visible;
  bool collision;
} Entity;

void move_entity(Entity *entity);
bool safe_move(Entity *entity);
void set_bounds(int top, int bottom, int left, int right);
bool collide(Entity *entity1, Entity *entity2);
void add_entity(Entity *entity);
Entity get_entity(int index);
int entity_count(void);
