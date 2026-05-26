#pragma once

typedef struct Map {
  const int ROWS;
  const int COLS;

  const char TOP;
  const char BOTTOM;
  const char LEFT;
  const char RIGHT;
  const char TOP_LEFT;
  const char TOP_RIGHT;
  const char BOTTOM_LEFT;
  const char BOTTOM_RIGHT;

  const char BACKGROUND;
} Map;

