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
  char *server_ip;
} Game;

void start(Game game);
void stop(void);
void toggle_verbose(void);
