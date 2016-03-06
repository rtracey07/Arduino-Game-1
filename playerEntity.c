#include "playerEntity.h"

bool 
player_create(player_t *player, int x, int y){
  bool status = false;

  player = (player_t*) malloc(sizeof(player_t));
  if(player){
    player->x = x;
    player->y = y;
    player->facing = UP;
    status = true;
  }
  return status;
}

void 
player_lock_vertical(player_t *player, bool v){
  if(player)
    player->move_vertical = v;
}

void 
player_lock_horizontal(player_t *player, bool h){
  if(player)
    player->move_horizontal = h;
}

void 
player_move(player_t *player, grid_t *grid, int facing, int distance){
  if(player && grid){
    switch(facing){
      case UP:
        if(player->move_vertical && player->y < grid->height)
          player->y++;  
        break;

      case DOWN:
        if(player->move_vertical && player->y>0)
          player->y--;
        break;

      case LEFT:
        if(player->move_horizontal && player->y>0)
          player->x--;
        break;  

      case RIGHT:
        if(player->move_horizontal && player->y< grid->width)
          player->x++;
        break;       
    }
  }
}

