#include "entity.h"
#include <unistd.h>

static Entity *entities[MAX_ENTITIES];
static Entity *col_entities[MAX_ENTITIES];
static int count = 0, col_count = 0;
static int LEFT_BOUND, TOP_BOUND;
static int RIGHT_BOUND, BOTTOM_BOUND;

void set_bounds(int top, int bottom, int left, int right) {
  TOP_BOUND = top;
  BOTTOM_BOUND = bottom;
  LEFT_BOUND = left;
  RIGHT_BOUND = right;
}

bool move(Entity *entity) {
  Entity temp = *entity;

  switch (temp.direction) {
  case UP:
    temp.y--;
    break;

  case DOWN:
    temp.y++;
    break;

  case LEFT:
    temp.x--;
    break;

  case RIGHT:
    temp.x++;
    break;

  case NONE:
    return false;
  }

  if (temp.y < TOP_BOUND || temp.y >= BOTTOM_BOUND ||
      temp.x < LEFT_BOUND || temp.x >= RIGHT_BOUND) {
    return true;
  }

  for (int i = 0; i < col_count; ++i) {
    Entity *other = get_col_entity(i);
    if (other != entity && collide(&temp, other)) {
      return true;
    }
  }

  *entity = temp;

  return false;
}

void add_entity(Entity *entity) {
  if (count < MAX_ENTITIES) {
    if ((*entity).collision)
      col_entities[col_count++] = entity;
    else
      entities[count++] = entity;
  }
}

Entity *get_entity(int index) {
  if (index < count) {
    return entities[index];
  }

  return NULL;
}

Entity *get_col_entity(int index) {
  if (index < col_count) {
    return col_entities[index];
  }

  return NULL;
}

int entity_count(void) { return count; }
int entity_col_count(void) { return col_count; }

bool collide(Entity *entity1, Entity *entity2) {
  if ((*entity1).collision && (*entity2).collision &&
      (*entity1).x == (*entity2).x && (*entity1).y == (*entity2).y) {
    return true;
  }

  return false;
}

bool collide_any(Entity *entity, Entity **entities, int size) {
  for (int i = 0; i < size; ++i)
    if ((*entity).x == (*entities[i]).x && (*entity).y == (*entities[i]).y)
      return true;
  return false;
}
