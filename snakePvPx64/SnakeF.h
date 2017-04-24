#ifndef SNAKEF_H
#define SNAKEF_H

#include "SDL.h"
#include "cmath"

extern SDL_Rect Snake_Box;
extern int speed;
typedef struct
{
	int x, y;
	int ox, oy;
	int img, previmg;
	const int w = 20;
	const int h = 20;
}snake;
typedef struct
{
	int x, y;
}Direction;
extern int odir[2], direction[2], winner;
extern int snakeLength[2], snakeLengthRect[2];
extern snake Snake[2][100];
extern Direction dir[4];
extern int level[2];
void Delay(int);
bool LoseGame(int);
void SetUpSnake();
bool SnakeCanMove(int);
void snakeControl(int);
bool SnakeEatSnake();
void snakeMove(int, int);
void snakeWall(int);
#endif