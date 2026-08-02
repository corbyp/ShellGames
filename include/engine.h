#pragma once

typedef struct Game {
  double time_delta;
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

  int client;
  int server;
} Game;

void start(int client, int server);
void stop(void);
void toggle_verbose(void);
