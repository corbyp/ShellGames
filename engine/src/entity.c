#include "entity.h"

static Entity *entities[MAX_ENTITIES];
static int count = 0;
static int LEFT_BOUND, TOP_BOUND;
static int RIGHT_BOUND, BOTTOM_BOUND;

void set_bounds(int top, int bottom, int left, int right) {
  TOP_BOUND = top;
  BOTTOM_BOUND = bottom;
  LEFT_BOUND = left;
  RIGHT_BOUND = right;
}

void move(Entity *entity) {
  switch ((*entity).direction) {
  case UP:
    (*entity).y--;
    break;

  case DOWN:
    (*entity).y++;
    break;

  case LEFT:
    (*entity).x--;
    break;

  case RIGHT:
    (*entity).x++;
    break;

  case NONE:
    break;
  }
}

bool safe_move(Entity *entity) {
  switch ((*entity).direction) {
  case UP:
    if ((*entity).y <= TOP_BOUND)
      return true;
    (*entity).y--;
    break;

  case DOWN:
    if ((*entity).y >= BOTTOM_BOUND - 1)
      return true;
    (*entity).y++;
    break;

  case LEFT:
    if ((*entity).x <= LEFT_BOUND)
      return true;
    (*entity).x--;
    break;

  case RIGHT:
    if ((*entity).x >= RIGHT_BOUND - 1)
      return true;

    (*entity).x++;
    break;

  case NONE:
    break;
  }

  return false;
}

void add_entity(Entity *entity) {
  if (count < MAX_ENTITIES)
    entities[count++] = entity;
}

Entity get_entity(int index) { return *entities[index]; }

int entity_count(void) { return count; }

bool collide(Entity *entity1, Entity *entity2) {
  if ((*entity1).collision && (*entity2).collision &&
      (*entity1).x == (*entity2).x && (*entity1).y == (*entity2).y) {
    return true;
  }

  return false;
}
