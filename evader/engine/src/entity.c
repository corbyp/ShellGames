#include "entity.h"

static Entity *entities[MAX_ENTITIES];
static int count = 0;

bool move_up(Entity *entity, int bound) {
  if ((*entity).y - 1 >= bound) {
    --(*entity).y;
    return false;
  }
  return true;
}

bool move_down(Entity *entity, int bound) {
  if ((*entity).y + 1 < bound) {
    ++(*entity).y;
    return false;
  }
  return true;
}

bool move_left(Entity *entity, int bound) {
  if ((*entity).x - 1 >= bound) {
    --(*entity).x;
    return false;
  }
  return true;
}

bool move_right(Entity *entity, int bound) {
  if ((*entity).x + 1 < bound) {
    ++(*entity).x;
    return false;
  }
  return true;
}

bool collide(Entity *entity1, Entity *entity2) {
  if ((*entity1).collision && (*entity2).collision &&
      (*entity1).x == (*entity2).x && (*entity1).y == (*entity2).y) {
    return true;
  }

  return false;
}

// external api
void add_entity(Entity *entity) {
  if (count < MAX_ENTITIES)
    entities[count++] = entity;
}

Entity get_entity(int index) {
  return *entities[index];
}

int entity_count(void) {
  return count;
}
