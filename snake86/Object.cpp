#include "SnakeF.h"
#include "PowerUps.h"
#include <cstdlib>
#include <ctime>
#include "Object.h"
#include "SDL_ttf.h"
#include "Sound.h"
#include "Render.h"


int window_w, window_h;
int snakeFruit;
bool obstacleState[2][40];
Uint32 last;
Uint32 previous;
Direction dir[4];
snake Snake[2][100];
fruit Fruit[2];
fruit Obstacles[2][40];
fruit Rock[3];
bool drop[2];
bool speedup[2];
bool eatFruit;
int dropped[2];

void SnakeEatFruit()
{
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			if (Snake[j][0].x == Fruit[i].x && Snake[j][0].y == Fruit[i].y)
			{
				eatFruit = true;
				PlayChunk();
				if (i == 0)
				{
					snakeFruit = j;
					IncreaseLength();	
				}
				else if (i == 1)
				{
					snakeFruit = 1 - j;
					DecreaseLength();
				}
			}
		}
	}
}

bool CanPlaceFruitHere(int i)
{
	for (int m = 0; m < 2; ++m)
	{
		for (int j = 0; j < snakeLength[m]; ++j)
		{
			if (Fruit[i].x == Snake[m][j].x && Fruit[i].y == Snake[m][j].y)
				return false;
		}
	}
	return true;
}

void MakeFruit()
{
	if (eatFruit == true)
	{
		srand(unsigned int(time(NULL)));
		for (int i = 0; i < 2; ++i)
		{
			for (; ;)
			{
				int x = rand() % 39;
				int y = rand() % 29;
				Fruit[i].x = x * 20 + Snake_Box.x + 20;
				Fruit[i].y = y * 20 + Snake_Box.y + 20;
				if (CanPlaceFruitHere(i) == true)
					break;
			}
		}
		eatFruit = false;
	}
}
// Make obstacle
void MakeObstacle()
{
	
	for (int j = 0; j < 2; ++j)
	{
		int num = (int(snakeLength[j] / 3));
		if (num > 10) num = 15;
		if (drop[j] == true)
		{
			for (int i = 0; i < num; ++i)
			{
				if (obstacleState[j][i] == false)
				{
					obstacleState[j][i] = true;
					Obstacles[j][i].x = Snake[j][snakeLength[j] - 1].ox;
					Obstacles[j][i].y = Snake[j][snakeLength[j] - 1].oy;
					drop[j] = false;
					snakeLength[j] -= 1;
					dropped[j] += 1;
					
					break;
				}
			}
		}
	}
}

bool SnakeEatObstacle(int Num)
{
	int num = (int(snakeLength[Num] / 3));
	if (num > 10) num = 15;
	for (int i = 0; i < num; ++i)
	{
		if (Snake[Num][0].x == Obstacles[1 - Num][i].x && Snake[Num][0].y == Obstacles[1 - Num][i].y && obstacleState[1 - Num][i] == true)
		{
			PlayChunk();
			obstacleState[1 - Num][i] = false;
			snakeFruit = Num;
			IncreaseLength();
			dropped[1 - Num] -= 1;
			return true;
		}
		if (Snake[Num][0].x == Obstacles[Num][i].x && Snake[Num][0].y == Obstacles[Num][i].y && obstacleState[Num][i] == true)
		{
			PlayChunk();
			obstacleState[Num][i] = false;
			snakeFruit = Num;
			DecreaseLength();
			dropped[Num] -= 1;
			return true;
		}
	}
	return false;
}

void Obstacle(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_m)
		{
			drop[0] = true;
		}
		else if (event.key.keysym.sym == SDLK_q)
		{
			drop[1] = true;
		}
	}
	MakeObstacle();
	for (int i = 0; i < 2; ++i)
	{
		if (SnakeEatObstacle(i))
		{ }
	}
}

bool CanPlaceRock(int i)
{
	for (int m = 0; m < 2; ++m)
	{
		for (int j = 0; j < snakeLength[m]; ++j)
		{
			if ((Rock[i].y <= Snake[m][j].y && Rock[i].y + 40 >= Snake[m][j].y) ||
				(Snake[m][j].y + Snake[m][j].h >= Rock[i].y && Snake[m][j].y + Snake[m][j].h <= Rock[i].y + 40))
				return false;
		}
	}
	return true;
}

void SetUpObject()
{
	window_h = 700;
	window_w = 1200;
	snakeFruit = 3;
	eatFruit = true;
	mute = false;
	for (int i = 0; i < 2; ++i)
	{
		drop[i] = false;
		swapdirection[i] = false;
		speedup[i] = false;
		level[i] = 1;
		dropped[i] = 0;
	}
	for (int i = 0; i < obstacleNum; ++i)
	{
		obstacleState[0][i] = false;
		obstacleState[1][i] = false;
	}
	pause = false;
	stop = false;
	//int ox = 0 , oy = 0;
	for (int i = 0; i < 3; ++i)
	{
		int x, y;
		for (; ;)
		{
			int n = rand() % 20;
			int m = rand() % 15;
			x = n * 35 + Snake_Box.x + 40;
			y = m * 30 + Snake_Box.y + 40;
			Rock[i].x = x;
			Rock[i].y = y;
			if (CanPlaceRock(i) == true)
			{
				break;
			}
		}
	}
}

void ContinueGame()
{
	SDL_Event occur;
	SDL_PollEvent(&occur);
	if (occur.type == SDL_KEYDOWN)
	{
		if (occur.key.keysym.sym == SDLK_SPACE)
		{
			SetUpSnake();
			SetUpObject();
		}
	}
}

bool SnakeinRock(int x, int y, fruit rect) {
	if (x > rect.x &&
		x < rect.x + 40 &&
		y > rect.y &&
		y < rect.y + 40
		)
	{
		return true;
	}

	return false;
}

bool CheckCollision(snake r1, fruit r2)
{
	if (SnakeinRock(r1.x, r1.y, r2) == true ||
		SnakeinRock(r1.x + r1.w, r1.y, r2) == true ||
		SnakeinRock(r1.x, r1.y + r1.h, r2) == true ||
		SnakeinRock(r1.x + r1.w, r1.y + r1.w, r2) == true)
		return true;
	return false;
}

void CollisionRock()
{
	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (CheckCollision(Snake[j][0], Rock[i]))
			{
				stop = true;
				winner = (1 - j);
			}
		}
	}
}