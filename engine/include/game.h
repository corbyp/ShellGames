#pragma once

typedef struct Game {
  int loop_delta;
  int timer;
} Game;

void start(void);
void stop(void);
void toggle_verbose(void);
void set_map_dims(const int rows, const int cols);
void set_map_chars(const char top, const char bottom, const char left, const char right, const char top_left, const char top_right, const char bottom_left, const char bottom_right, const char background);
