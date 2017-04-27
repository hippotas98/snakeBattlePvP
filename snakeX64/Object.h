
#ifndef OBJECT_H
#define OBJECT_H

#include "SnakeF.h"
#include "PowerUps.h"
#include <cstdlib>
#include <ctime>

extern bool stop;
extern bool pause;
extern int window_w;
extern int window_h;

typedef struct
{
	int x, y;
	const int w = 20;
	const int h = 20;
}fruit;

const int obstacleNum = 40;
extern fruit Fruit[2];
extern fruit Obstacles[2][40];
extern fruit Rock[3];
extern int snakeFruit;
extern bool obstacleState[2][40];
extern bool eatFruit;
extern bool drop[2];
extern int dropped[2];


bool CanPlaceFruitHere(int);
void ContinueGame();
void MakeFruit();
void Obstacle(SDL_Event);
void SetUpObject();
void SetUpObject();
void SnakeEatFruit();
bool SnakeEatObstacle(int);
bool SnakeinRock(int, int, fruit);
bool CheckCollision(snake, fruit);
void CollisionRock();
bool CanPlaceRock(int );


#endif