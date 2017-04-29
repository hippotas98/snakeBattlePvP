#include "SnakeF.h"
#include "Object.h"
#include "cstdlib"
#include "SDL.h"
#include "SDL_ttf.h"
#include "PowerUps.h"
#include "Render.h"
//bool speedup[2];
void increaseLengthbyN(int n)
{
	for (int i = 0; i < n; ++i)
	{
		if (snakeFruit != 3)
		{
			Snake[snakeFruit][snakeLength[snakeFruit]].x = Snake[snakeFruit][snakeLength[snakeFruit] - 1].ox;
			Snake[snakeFruit][snakeLength[snakeFruit]].y = Snake[snakeFruit][snakeLength[snakeFruit] - 1].oy;
			Snake[snakeFruit][snakeLength[snakeFruit]].img = Snake[snakeFruit][snakeLength[snakeFruit] - 1].previmg;
			snakeLength[snakeFruit] += 1;
			snakeLengthRect[snakeFruit] += 1;
		}
		if (i == n - 1)
			snakeFruit = 3;
	}
}

void DecreaseLengthbyN(int n)
{
	if (snakeFruit != 3)
	{
		snakeLength[snakeFruit] -= n;
		snakeFruit = 3;
	}
}

void IncreaseLengthbyOne()
{
	increaseLengthbyN(1);
}

void IncreaseLengthbyTwo()
{
	increaseLengthbyN(2);
}

void DecreaseLengthbyOne()
{
	DecreaseLengthbyN(1);
}

void DecreaseLengthbyTwo()
{
	DecreaseLengthbyN(2);
}

void swapLength()
{
	int different = abs(snakeLength[0] - snakeLength[1]);
	if (snakeLength[0] > snakeLength[1])
	{
		snakeLength[0] -= different;
		if (snakeFruit == 0)
		{
			snakeFruit = 1;
		}
		increaseLengthbyN(different);
	}
	else if (snakeLength[1] > snakeLength[0])
	{
		snakeLength[1] -= different;
		if (snakeFruit == 1)
		{
			snakeFruit = 0;
		}
		increaseLengthbyN(different);
	}
}

void Backto5()
{
	if (snakeFruit != 3)
	{
		int diff = abs(5 - snakeLength[snakeFruit]);
		if (snakeLength[snakeFruit] > 5)
			DecreaseLengthbyN(diff);
		else if (snakeLength[snakeFruit] < 5)
			increaseLengthbyN(diff);
		snakeFruit = 3;
	}
}

void HalvetheLength()
{
	if (snakeFruit != 3)
	{
		int half = int(snakeLength[snakeFruit] / 2);
		DecreaseLengthbyN(half);
	}
}

void swapDirection()
{
	if (snakeFruit != 3)
	{
		last = SDL_GetTicks();
		if (swapdirection[snakeFruit] == 0)
			swapdirection[snakeFruit]= 1;
	}
}

void Continue()
{
	for (int i = 0; i < 2; ++i)
	{
		if (swapdirection[i] == 1)
			swapdirection[i] = 0;
	}
}

void StopSwap()
{
	uint32_t now;
	now = SDL_GetTicks();
	uint32_t delta = now - last;
	if (delta > 6000)
	{
		Continue();
	}
}

void IncreaseLength()
{
	int randomnumber = rand() % 5;
	switch (randomnumber)
	{
	case 0: IncreaseLengthbyOne();
		break;
	case 1: IncreaseLengthbyOne();
		break;
	case 2: IncreaseLengthbyTwo();
		break;
	case 3: IncreaseLengthbyTwo();
		break;
	case 4: swapLength();
		break;
	}
}

void DecreaseLength()
{
	int random = rand() % 6 + 1;
	switch (random)
	{
	case 1: DecreaseLengthbyOne();
		break;
	case 2: DecreaseLengthbyTwo();
		break;
	case 3: Backto5();
		break;
	case 4: HalvetheLength();
		break;
	case 5: swapLength();
		break;
	case 6:swapDirection();
		break;
	}
}