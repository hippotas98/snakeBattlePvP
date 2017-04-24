
#include "SDL.h"
#include "cmath"
#include "SnakeF.h"
#include "Object.h"
int speed;
bool swapdirection[2];
void Delay(int level) {
	switch (level)
	{
	case 1: SDL_Delay(30);
		break;
	case 2: SDL_Delay(25);
		break;
	case 3: SDL_Delay(20);
		break;
	case 4: SDL_Delay(15);
		break;
	case 5: SDL_Delay(10);
		break;
	}
}

bool SnakeCanMove(int snakeNumber) {
	if (abs(odir[snakeNumber] - direction[snakeNumber]) == 2) return false;
	return true;
}

void snakeMove(int direction, int number)
{
	for (int i = 0; i < snakeLengthRect[number]; ++i)
	{
		if (i == 0)
		{
			Snake[number][i].ox = Snake[number][i].x;
			Snake[number][i].oy = Snake[number][i].y;
			Snake[number][i].x += dir[direction].x;
			Snake[number][i].y += dir[direction].y;
			Snake[number][i].previmg = Snake[number][i].img;
			Snake[number][i].img = direction;
		}
		else
		{
			Snake[number][i].ox = Snake[number][i].x;
			Snake[number][i].oy = Snake[number][i].y;
			Snake[number][i].x = Snake[number][i - 1].ox;
			Snake[number][i].y = Snake[number][i - 1].oy;
			Snake[number][i].previmg = Snake[number][i].img;
			Snake[number][i].img = Snake[number][i - 1].previmg;
		}

	}
	Delay(level[number]);
}

void snakeControl(int i)
{
	const Uint8 *key = SDL_GetKeyboardState(NULL);
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (SnakeCanMove(i) == 1)
		odir[i] = direction[i];
	if ((i == 0 && key[SDL_SCANCODE_UP])
		|| i == 1 && state[SDL_SCANCODE_W])
	{
		direction[i] = 0 + 2*swapdirection[i];
	}
	else if ((i == 0 && key[SDL_SCANCODE_DOWN])
		|| i == 1 && state[SDL_SCANCODE_S])
	{
		direction[i] = 2 - 2*swapdirection[i];
	}
	else if ((i == 0 && key[SDL_SCANCODE_RIGHT])
		|| i == 1 && state[SDL_SCANCODE_D])
	{
		direction[i] = 1 + 2 * swapdirection[i];
	}
	else if ((i == 0 && key[SDL_SCANCODE_LEFT])
		|| i == 1 && state[SDL_SCANCODE_A])
	{
		direction[i] = 3 - 2 * swapdirection[i];
	}
	if (SnakeCanMove(i) == true)
		snakeMove(direction[i], i);
	else
		snakeMove(odir[i], i);
}

void snakeWall(int number)
{
	if (Snake[number][0].x + Snake[number][0].w > Snake_Box.x + Snake_Box.w) Snake[number][0].x = Snake_Box.x;
	else if (Snake[number][0].x < Snake_Box.x) Snake[number][0].x = Snake_Box.x + Snake_Box.w - Snake[number][0].w;
	if (Snake[number][0].y + Snake[number][0].h > Snake_Box.y + Snake_Box.h) Snake[number][0].y = Snake_Box.y;
	else if (Snake[number][0].y < Snake_Box.y) Snake[number][0].y = Snake_Box.y + Snake_Box.h - Snake[number][0].h;
}

bool LoseGame(int number)
{
	for (int i = 1; i < snakeLength[number]; ++i)
	{
		if (Snake[number][0].x == Snake[number][i].x  && Snake[number][0].y == Snake[number][i].y)
		{
			return true;
		}
	}
	if (snakeLength[number] < 2) return true;
	if (snakeLength[number] > 80) return true;
	return false;
}

bool SnakeEatSnake()
{
	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < snakeLength[1-j]; ++i)
		{
			if (Snake[j][0].x == Snake[abs(1 - j)][i].x  && Snake[j][0].y == Snake[abs(1 - j)][i].y)
			{
				if (snakeLength[j] >= snakeLength[abs(1 - j)])
				{
					winner = j;
				}
				else
				{
					winner = abs(1 - j);
				}
				return true;
			}
		}
	}
	return false;
}

void SetUpSnake()
{
	Snake_Box = { 200,50,800,600 };
	speed = 20;
	odir[0] = 1;
	odir[1] = 3;
	direction[0] = 3;
	direction[1] = 1;
	snakeLength[0] = 5;
	snakeLengthRect[0] = 5;
	snakeLength[1] = 5;
	snakeLengthRect[1] = 5;

	dir[0].x = 0;
	dir[0].y = -speed;

	dir[1].x = speed;
	dir[1].y = 0;

	dir[2].x = 0;
	dir[2].y = speed;

	dir[3].x = -speed;
	dir[3].y = 0;

	for (int i = 0; i < snakeLengthRect[0]; ++i)
	{
		Snake[0][i].x = Snake_Box.x + 200 + speed*(snakeLength[0] - i);
		Snake[0][i].y = Snake_Box.y + 400;
		Snake[0][i].img = direction[1];
	}
	for (int i = 0; i < snakeLengthRect[1]; ++i)
	{
		Snake[1][i].x = Snake_Box.x + speed*i + 20;
		Snake[1][i].y = Snake_Box.y + 60;
		Snake[1][i].img = direction[0];
	}
	winner = 3;
}
