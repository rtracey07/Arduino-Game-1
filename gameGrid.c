#include "gameGrid.h"

bool grid_create(grid_t* grid, int height, int width){
  bool created = false;

  grid = (grid_t *) malloc(sizeof(grid_t));

  if(grid && height >0 && width > 0){
    grid->height = height;
    grid->width = width;
    created = true;
  }
  return created;
}

