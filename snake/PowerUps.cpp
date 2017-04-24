#include "SnakeF.h"
#include "Object.h"
#include "cstdlib"
#include "SDL.h"
#include "SDL_ttf.h"
#include "PowerUps.h"

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

void DecreaseLengthByTwo()
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

void Backto8()
{
	if (snakeFruit != 3)
	{
		int diff = abs(8 - snakeLength[snakeFruit]);
		if (snakeLength[snakeFruit] > 8)
			DecreaseLengthbyN(diff);
		else if (snakeLength[snakeFruit] < 8)
			increaseLengthbyN(diff);
		snakeFruit = 3;
	}
}

void HalvetheLengh()
{
	if (snakeFruit != 3)
	{
		int half = int(snakeLength[snakeFruit] / 2);
		DecreaseLengthbyN(half);
	}
}

void Stunning()
{
	if (snakeFruit != 3)
	{
		accelerator[snakeFruit] = 0;
		last = SDL_GetTicks();
	}
}

void Continue()
{
	for (int i = 0; i < 2; ++i)
	{
		if (accelerator[i] == 0)
			accelerator[i] = 1;
	}
}

void StoptheSnake()
{
	uint32_t now;
	now = SDL_GetTicks();
	uint32_t delta = now - last;
	if (delta > 1000)
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
	int random = rand() % 7 + 1;
	switch (random)
	{
	case 1: DecreaseLengthbyOne();
		break;
	case 2: DecreaseLengthByTwo();
		break;
	case 3: Backto8();
		break;
	case 4: HalvetheLengh();
		break;
	case 6: swapLength();
		break;
	case 7:Stunning();
		break;
	}
}