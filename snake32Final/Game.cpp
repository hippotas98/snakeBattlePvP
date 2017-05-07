#include "SnakeF.h"
#include "Object.h"
#include "PowerUps.h"
#include "Render.h"
#include "iostream"
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <SDL_mixer.h>
#include "Sound.h"
#include "Menu.h"

SDL_Color White = { 255,255,255 };
int attention;

void Harder()
{
	int sum = snakeLength[0] + snakeLength[1];
	for (int i = 0; i < 2; ++i)
	{
		if (sum < 20)
			level[i] = 1;
		else if (sum >= 20 && sum <= 30)
			level[i] = 2;
		else if (sum > 30 && sum <= 40)
			level[i] = 3;
		else if (sum  > 40)
			level[i] = 4;
	}

}

void Logic()
{
	MakeFruit();

	for (int i = 0; i < 2; ++i)
	{
		snakeControl(i);
		snakeWall(i);
		if (Prediction(direction[i], i) == true)
		{
			if(snakeLength[i] >= snakeLength[1-i])
			attention = 1 - i;
			else attention = i;
		}
		else attention = 2;
		if (LoseGame(i))
			stop = true;
	}

	Obstacle(event);

	if (SnakeEatSnake() == true)
	{
		stop = true;
	}

	SnakeEatFruit();
	CollisionRock();
	if (swapdirection[0] == 1 || swapdirection[1] == 1)
	{
		StopSwap();
	}
	Harder();
}

void Event(bool *running)
{
	PauseGame(running);
	Music();
}

int main(int argv, char ** args)
{
	bool running = true;
	bool start = false;
	LoadGame();
	int menu;
	while (!start)
	{
		menu = GameMenu();
		Instruction(&menu, &start);
	}
	if (menu == 0)
	{
		//PlaySound();
		while (running)
		{
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT)
			{
				running = false;
				Quit();
				break;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
					Quit();
					break;
				}
			}
			Event(&running);
			if (running == false)
				break;
			if (pause == false)
			{
				Logic();
			}
			DrawScreen();
		}
	}
	else if (menu == 1)
	{
		Quit();
	}
	return 0;
}