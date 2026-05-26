#pragma once

typedef struct Game {
  int loop_delta;
  unsigned long timer;
  const int ROWS;
  const int COLS;
} Game;

void start(void);
void stop(void);
void toggle_verbose(void);
