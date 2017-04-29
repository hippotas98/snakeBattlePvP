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


SDL_Event event;
SDL_Color White = { 255,255,255 };
//void Quit();

void Pause(bool *running)
{
	SDL_PollEvent(&event);
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		int x = event.motion.x;
		int y = event.motion.y;
		if (x > 450 + 81 && x < 450 + 220 && y > 195 + 130 && y < 195 + 161)
		{
			pause = false;
			//break;
		}
		else if (x > 450 + 81 && x < 450 + 220 && y > 195 + 192 && y < 195 + 225)
		{
			pause = false;
			SetUpObject();
			SetUpSnake();
			//break;
		}
		else if (x > 450 + 81 && x < 450 + 220 && y > 195 + 256 && y < 195 + 286)
		{
			pause = false;
			*running = false;
			Quit();
		}
	}
	else if (event.type == SDL_QUIT)
	{
		pause = false;
		*running = false;
		Quit();
	}
}

bool SelectPause()
{
	const int window_w = 1200;
	const int window_h = 700;
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		int x = event.motion.x;
		int y = event.motion.y;
		if (x > window_w / 2 - 25 && x < window_w / 2 + 25 && y > 0 && y < 50)
		{
			return true;
		}
	}
	return false;
}

bool SelectMute()
{
	const int window_w = 1200;
	const int window_h = 700;
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		int mx = event.motion.x;
		int my = event.motion.y;
		if (mx > window_w/2 + 50  && mx < window_w / 2 + 100 && my > 0 && my < 50)
		{
			return true;
		}
	}
	return false;
}

void Music()
{
	if (SelectMute())
	{
		if (mute == true)
			mute = false;
		else if(mute == false) mute = true;
		if (Mix_PausedMusic() == 1)
		{
			if (mute == false)
			{
				Mix_ResumeMusic();
			}
		}
		else
		{
			if (mute == true)
			{
				Mix_PauseMusic();
			}
		}
	}
}

void PauseGame(bool *running)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_p)
		{
			if (!pause)
			{
				pause = true;
				DrawPauseMenu();
				DrawPauseButton();
				SDL_RenderPresent(renderer);
				while (pause)
				{
					Pause(running);
					if (event.type == SDL_KEYDOWN)
						if (event.key.keysym.sym == SDLK_p)
							pause = false;
				}
			}
		}
	}
	if (pause == false)
	{
		if (SelectPause() == true)
		{
			pause = true;
			DrawPauseMenu();
			DrawPauseButton();
			SDL_RenderPresent(renderer);
			while (pause == true)
			{
				Pause(running);
			}
		}
	}
}

int GameMenu() {

	Uint32 time;
	int mousex, mousey;
	const int NumofMenu = 2;
	const char *Menu[NumofMenu] = { "Start","Exit" };
	SDL_Surface *MenuSur[NumofMenu];
	SDL_Texture *MenuText[NumofMenu];
	SDL_Rect MenuRect[NumofMenu];
	SDL_Rect Screen = { 0,0,window_w, window_h };
	SDL_Surface *Background;
	SDL_Color red = { 255,0,0 };
	SDL_Color green = { 0,153,0 };
	bool Selected[NumofMenu];

	for (int i = 0; i < NumofMenu; ++i) {
		Selected[i] = 0;
	}

	MenuRect[0] = { 670,345,120,70 };
	MenuRect[1] = { 680,460,100,70 };

	IMG_Init(IMG_INIT_PNG);
	Background = IMG_Load("./img/menu.png");
	if (Background == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Err", SDL_GetError(), windows);
	}
	RenderText(Background, &Screen);
	for (int i = 0; i < 2; ++i)
	{
		MenuSur[i] = TTF_RenderText_Solid(comic, Menu[i], green);
		MenuText[i] = SDL_CreateTextureFromSurface(renderer, MenuSur[i]);
		SDL_RenderCopy(renderer, MenuText[i], NULL, &MenuRect[i]);
	}
	SDL_RenderPresent(renderer);
	SDL_Event menuevent;
	while (true)
	{
		time = SDL_GetTicks();
		while (SDL_PollEvent(&menuevent)) {
			switch (menuevent.type)
			{
			case SDL_QUIT:
				for (int i = 0; i < 2; ++i)
				{
					SDL_FreeSurface(MenuSur[i]);
					SDL_DestroyTexture(MenuText[i]);
				}
				Quit();
				break;
				return 1;
			case SDL_MOUSEMOTION:
				mousex = menuevent.motion.x;
				mousey = menuevent.motion.y;
				for (int i = 0; i < NumofMenu; ++i)
				{
					if (MenuRect[i].x < mousex &&
						MenuRect[i].x + MenuRect[i].w > mousex &&
						MenuRect[i].y < mousey &&
						MenuRect[i].y + MenuRect[i].h > mousey)
					{
						if (Selected[i] == 0)
						{
							Selected[i] = 1;
							SDL_DestroyTexture(MenuText[i]);
							SDL_Surface *Temp = TTF_RenderText_Solid(comic, Menu[i], red);
							MenuText[i] = SDL_CreateTextureFromSurface(renderer, Temp);
							SDL_FreeSurface(Temp);
						}
					}
					else
					{
						if (Selected[i] == 1)
						{
							Selected[i] = 0;
							SDL_DestroyTexture(MenuText[i]);
							SDL_Surface *Temp = TTF_RenderText_Solid(comic, Menu[i], green);
							MenuText[i] = SDL_CreateTextureFromSurface(renderer, Temp);
							SDL_FreeSurface(Temp);
						}
					}
					SDL_RenderCopy(renderer, MenuText[i], NULL, &MenuRect[i]);
					SDL_RenderPresent(renderer);
					SDL_DestroyTexture(MenuText[i]);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
			{
				mousex = menuevent.motion.x;
				mousey = menuevent.motion.y;
				for (int i = 0; i < NumofMenu; ++i)
				{
					if (MenuRect[i].x < mousex &&
						MenuRect[i].x + MenuRect[i].w > mousex &&
						MenuRect[i].y < mousey &&
						MenuRect[i].y + MenuRect[i].h > mousey)
					{
						for (int i = 0; i < 2; ++i)
						{
							SDL_FreeSurface(MenuSur[i]);
							SDL_DestroyTexture(MenuText[i]);
						}
						return i;
					}
				}
				break;
			}
			case SDL_KEYDOWN:
			{
				if (menuevent.key.keysym.sym == SDLK_ESCAPE)
				{
					for (int i = 0; i < 2; ++i)
					{
						SDL_FreeSurface(MenuSur[i]);
						SDL_DestroyTexture(MenuText[i]);
					}
					return 1;
				}
				else if (menuevent.key.keysym.sym == SDLK_RETURN)
				{
					return 0;
				}
			}
			}
		}
	}

	if (30 > (SDL_GetTicks() - time)) {
		SDL_Delay(30 > (SDL_GetTicks() - time));
	}
	return 0;
}

void Instruction(int *menu, bool *start)
{
	//IMG_Init(IMG_INIT_PNG);
	SDL_Rect screen = { 0,0,1200,700 };
	SDL_Surface *instruction = IMG_Load("./img/instruction.png");
	SDL_Texture *instr = SDL_CreateTextureFromSurface(renderer, instruction);
	bool running = true;
	if (*menu == 0)
	{
		while (running)
		{
			SDL_Event occur;
			SDL_PollEvent(&occur);
			if (occur.type == SDL_MOUSEBUTTONDOWN)
			{
				int x = occur.motion.x;
				int y = occur.motion.y;
				if (x >= 1052 && x <= 1052 + 94 && y >= 96 && y <= 95 + 55)
				{
					running = false;
					*start = false;
					*menu = 1;
				}
			}
			SDL_RenderCopy(renderer, instr, NULL, &screen);
			SDL_RenderPresent(renderer);
			if (occur.type == SDL_KEYDOWN)
			{
				if (occur.key.keysym.sym == SDLK_ESCAPE)
				{
					*start = true;
					running = false;
					*menu = 1;
				}
				running = false;
				*start = true;
			}
			else if (occur.type == SDL_QUIT)
			{
				*start = true;
				running = false;
				*menu = 1;
			}
		}
	}
	else if (*menu == 1)
	{
		*start = true;
		running = false;
	}
	SDL_DestroyTexture(instr);
	SDL_FreeSurface(instruction);
}

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
	}
	Obstacle(event);
	if (LoseGame(0) == true)
	{
		stop = true;
		winner = 0;
	}
	if (LoseGame(1) == true)
	{
		stop = true;
		winner = 1;
	}
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
		PlaySound();
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