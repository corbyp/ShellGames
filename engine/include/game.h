#pragma once

typedef struct Game {
  int loop_delta;
  int timer;

  int rows;
  int cols;

  char top;
  char bottom;
  char left;
  char right;
  char top_left;
  char top_right;
  char bottom_left;
  char bottom_right;

  char background;
} Game;

void start(void);
void stop(void);
void toggle_verbose(void);
