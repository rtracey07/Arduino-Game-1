/** Simple Space Invaders-esque videogame.  
 *  Uses Adafruit 8x8 NeoPixel Matrix.
 *  Programmed with the matrix turned sideways (for wiring purposes). */

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "playerEntity.h"
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 6

//Screen Boundaries:
#define WIDTH 7           //Game field width.
#define W_I 6             //Max width index.
#define HEIGHT 8          //Game filed height.
#define H_I 7             //Max height index.

#define RATE 9000         //Refresh rate.
#define ENEMY_SPEED 17    //Spawn rate of enemies.

//Controls:
#define REPLAY 'v'        //Reset button.

//Score Values:
#define WIN_SCORE 8       //Number of points to win.
#define INC_SCORE 4       //Number of enemies destroyed to inc points by 1.

//Game States:
#define PLAY 0
#define WIN 1
#define LOSE 2
#define RESET 3

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

  
byte serialIn = 0;        //Keyboard input via USB.
int playerPosition = 0;   //Character position.

int frame = 0;            //Framerate counter.
int newEnemy = 0;         //Enemy spawn counter.
int score = 0;            //Score counter.
int hit = 0;              //Enemy hit counter.

//Coordinates for enemies and missiles.
int missiles[HEIGHT][WIDTH];  
int enemies[HEIGHT][WIDTH];

//Start state.
int state = PLAY;

void setup() {
  matrix.begin();
  Serial.begin(9600);
  matrix.setBrightness(40);
  matrix.fillScreen(0);
}

void loop() {
  frame++;  
  if(frame == RATE){
    frame = 0;

    //Check for restart.  
    serialIn = Serial.available() ? Serial.read() : 0;     
    if(serialIn == REPLAY)
      state = RESET;
         
    switch(state){       
      case PLAY:
        runGame();
        break;

      case WIN:
        winScreen();
        break;

      case LOSE:
        gameOverScreen();
        break;

      case RESET:
        reset();
        break;  
    }
  }
}

/** Get user input, spawn enemies, fire, and keep score.  */
void runGame(){

  //Spawn new Enemy.
  newEnemy ++;     
  if(newEnemy == ENEMY_SPEED){
    newEnemy=0;
    enemies[0][random(0,WIDTH)]=1;
  }

  //Move player/fire.
  if(serialIn != 0)
    playerAction();

  //Update screen.
  matrix.fillScreen(0);
  matrix.drawPixel(H_I, playerPosition, matrix.Color(0,60,255));
  updateMissiles();
  updateScore();

  //Update enemy (move at half spawn rate frame).
  if(newEnemy == ENEMY_SPEED/2)
    updateEnemies();
  else
    showEnemies();
        
  checkGameOver();
  matrix.show();
}

/** Refresh game counters and restore to start state. */
void reset(){
  score = 0;
  hit = 0;
  playerPosition = 0;
  newEnemy = 0;
  
  for(int i=0; i<HEIGHT; i++)
    for(int j=0; j<WIDTH; j++)
      enemies[i][j] = 0;

  state = PLAY;    
}

/** Get user input for move or fire.  */
void playerAction(){
     if(serialIn == RIGHT && playerPosition <W_I)
      playerPosition ++;   
     else if(serialIn == LEFT && playerPosition > 0)
      playerPosition --;
     else if(serialIn == FIRE)
      missiles[W_I][playerPosition]=1;
}

/** Move missiles to new positions and check for enemy hits.  */
void updateMissiles(){
  for(int i=0; i<HEIGHT; i++){
    for(int j=0; j<WIDTH; j++){
      if(missiles[i][j] == 1 && enemies[i][j] == 1){
        missiles[i][j]=0;
        enemies[i][j]=0;
        hit++;
        if(hit==INC_SCORE){
          score++;
          hit =0;
        }
      }
      else if(missiles[i][j] == 1){
        matrix.drawPixel(i,j, matrix.Color(255,60,0));
        missiles[i][j]=0;
        if(i!=0)
          missiles[i-1][j]=1;
      }
    }
  }
}

/** Move enemies to new position.  */
void updateEnemies(){
  for(int i=H_I; i>=0; i--)
    for(int j=W_I; j>=0; j--)
      if(enemies[i][j] == 1){
        matrix.drawPixel(i,j, matrix.Color(255,255,0));
        enemies[i][j]=0;
        if(i!=H_I)
          enemies[i+1][j]=1;
      }
}

/** Refresh enemy position (not moved). */
void showEnemies(){
  for(int i=H_I; i>=0; i--)
    for(int j=W_I; j>=0; j--)
      if(enemies[i][j] == 1)
        matrix.drawPixel(i,j, matrix.Color(255,255,0));
}

/** If enemy reaches last row, game over.  */
void checkGameOver(){
  for(int i=0; i<WIDTH; i++)
    if(enemies[H_I][i]==1)
      state = LOSE;
}

/** Sad face game over screen.  */
void gameOverScreen(){
  int o = matrix.Color(100,100,0);
  int x = matrix.Color(255,0,0);
  int sadFace[8][8] = {
    {o,o,o,o,o,o,o,o},
    {x,o,x,o,o,x,o,x},
    {o,x,o,o,o,o,x,o},
    {x,o,x,o,o,x,o,x},
    {o,o,o,o,o,o,o,o},
    {o,o,x,x,x,x,o,o},
    {o,x,o,o,o,o,x,o},
    {o,o,o,o,o,o,o,o}
  };

  for(int i=0; i<8; i++)
    for(int j=0; j<8; j++)
      matrix.drawPixel(i,j, sadFace[i][j]);

  matrix.show();  
}

/** Happy face win screen.  */
void winScreen(){
  int o = matrix.Color(0,120,40);
  int x = matrix.Color(180,180,180);
  int happyFace[8][8] = {
    {o,o,o,o,o,o,o,o},
    {o,x,o,o,o,o,x,o},
    {x,x,x,o,o,x,x,x},
    {o,x,o,o,o,o,x,o},
    {o,o,o,o,o,o,o,o},
    {o,x,o,o,o,o,x,o},
    {o,o,x,x,x,x,o,o},
    {o,o,o,o,o,o,o,o}
  };

  for(int i=0; i<8; i++)
    for(int j=0; j<8; j++)
      matrix.drawPixel(i,j, happyFace[i][j]);

  matrix.show();  
}

/** Increase score.  */
void updateScore(){
  if(score == WIN_SCORE)
    state = WIN;
  else  
    for(int i=0; i<score; i++)
      matrix.drawPixel(i,7,matrix.Color(0,255,100));
}

