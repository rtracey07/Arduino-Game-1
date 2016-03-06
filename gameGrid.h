#ifndef _gameGrid_
#define _gameGrid_

#include <stdbool.h>

typedef struct {
  int height;
  int width;
} grid_t;

bool grid_create(grid_t* grid, int height, int width);

#endif

