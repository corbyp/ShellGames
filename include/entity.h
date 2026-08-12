#pragma once

#include <stdbool.h>

#define MAX_ENTITIES 10000

typedef enum Type {
  DUMMY,

  PLAYER,
  NEUTRAL,
  ALLY,
  ENEMY,
  
  ITEM,
  PROJECTILE,

  ENVIRONMENT,
  WALL,
  DOOR,
  CHECKPOINT
  
} Type;

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
  char icon;

  Direction direction;
  Type type;

  bool visible;
  bool collision;
} Entity;

Type move(Entity *entity);
void set_bounds(int top, int bottom, int left, int right);
bool collide(Entity *entity1, Entity *entity2);
bool collide_any(Entity *entity1, Entity **entities, int size);
bool will_collide(Entity *entity1, Entity *entity2);
void add_entity(Entity *entity);
Entity *get_entity(int index);
Entity *get_col_entity(int index);
int entity_count(void);
int entity_col_count(void);
