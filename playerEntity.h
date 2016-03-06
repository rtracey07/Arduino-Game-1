#ifndef _playerEntity_
#define _playerEntity_

#include <stdbool.h>
#include "gameGrid.h"

#define UP 65
#define DOWN 66
#define LEFT 68
#define RIGHT 67
#define FIRE 32

typedef struct {
  int x;
  int y;
  bool move_horizontal;
  bool move_vertical;
  int facing;
} player_t;

bool player_create(player_t *player, int xPos, int yPos);
void player_lock_vertical(player_t *player, bool v);
void player_lock_horizontal(player_t *player, bool h);
void player_move(player_t *player, grid_t *grid, int facing, int distance);

#endif
