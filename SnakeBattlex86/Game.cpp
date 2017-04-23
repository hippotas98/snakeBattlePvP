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
void Quit();


int Pause() {
	int input;
	const SDL_MessageBoxButtonData button[] = {
		{ 0,0,"Restart" },
		{ 0,1,"Continue" }
	};
	const SDL_MessageBoxColorScheme colorScheme[5] = {
		{ 255,255,255 }, //background color
		{ 0,0,0 }, //Text color
		{ 255,255,0 }, //button color
		{ 255,255,255 }, //button background
		{ 0,0,0 } //button text
	};
	const SDL_MessageBoxData messageboxdata{
		SDL_MESSAGEBOX_INFORMATION,
		NULL,
		"Pause",
		"Player 1 use Arrow Key to Move the Snake\nPlayer 2 use A S W D to Move the Snake",
		2,
		button,
		&colorScheme[5],
	};
	if (SDL_ShowMessageBox(&messageboxdata, &input) < 0) {
		SDL_Log("error displaying message box");
		return -1;
	}
	return input;
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

	MenuRect[0].x = 670;
	MenuRect[1].x = 680;
	MenuRect[0].y = 345;
	MenuRect[1].y = 350 + 110;

	MenuRect[0].h = 70;
	MenuRect[1].h = 70;
	MenuRect[0].w = 120;
	MenuRect[1].w = 100;
	MenuSur[0] = TTF_RenderText_Solid(times, Menu[0], green);
	MenuSur[1] = TTF_RenderText_Solid(times, Menu[1], green);

	IMG_Init(IMG_INIT_PNG);
	Background = IMG_Load("./img/menu.png");
	if (Background == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Err", SDL_GetError(), windows);
	}
	RenderText(Background, &Screen);

	MenuText[0] = SDL_CreateTextureFromSurface(renderer, MenuSur[0]);
	MenuText[1] = SDL_CreateTextureFromSurface(renderer, MenuSur[1]);
	SDL_RenderCopy(renderer, MenuText[0], NULL, &MenuRect[0]);
	SDL_RenderCopy(renderer, MenuText[1], NULL, &MenuRect[1]);
	SDL_RenderPresent(renderer);
	SDL_Event menuevent;
	while (true)
	{
		time = SDL_GetTicks();
		while (SDL_PollEvent(&menuevent)) {
			switch (menuevent.type)
			{
			case SDL_QUIT:
				SDL_FreeSurface(MenuSur[0]);
				SDL_DestroyTexture(MenuText[0]);
				SDL_FreeSurface(MenuSur[1]);
				SDL_DestroyTexture(MenuText[1]);
				Quit();
				break;
				return 2;
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
							SDL_Surface *Temp = TTF_RenderText_Solid(times, Menu[i], red);
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
							SDL_Surface *Temp = TTF_RenderText_Solid(times, Menu[i], green);
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
						SDL_FreeSurface(MenuSur[0]);
						SDL_DestroyTexture(MenuText[0]);
						SDL_FreeSurface(MenuSur[1]);
						SDL_DestroyTexture(MenuText[1]);
						return i;
					}
				}
				break;
			}
			case SDL_KEYDOWN:
			{
				if (menuevent.key.keysym.sym == SDLK_ESCAPE)
				{
					Quit();
					break;
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
	if (sum < 20)
		level = 1;
	else if (sum >= 20 && sum <= 30)
		level = 2;
	else if (sum > 30 && sum <= 40)
		level = 3;
	else if (sum  > 40)
		level = 4;
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
	if (accelerator[0] == 0 || accelerator[1] == 0)
		StoptheSnake();
	Harder();
}

void Event(bool running)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_p)
		{
			if (!pause)
				pause = true;
			else if (pause)
				pause = false;
		}
	}
	if (pause == false)
	{
		if (SelectPause() == true)
		{
			pause = true;
			DrawPauseButton();
			if (Pause() == 1)
			{
				pause = false;
			}
			else
			{
				pause = false;
				SetUpSnake();
				SetUpObject();
			}
		}
	}
}

void Quit()
{
	QuitSound();
	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 14; ++i)
		{
			SDL_DestroyTexture(snake_Txt[j][i]);
			SDL_FreeSurface(snake_Sur[j][i]);
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		SDL_DestroyTexture(background_Txt[i]);
		SDL_FreeSurface(background_Sur[i]);
	}
	SDL_FreeSurface(rock_Sur);
	SDL_DestroyTexture(rock_Txt);
	SDL_VideoQuit();
	SDL_FreeSurface(play_button);
	SDL_DestroyTexture(play_txt);
	SDL_FreeSurface(pause_button);
	SDL_DestroyTexture(pause_txt);
	SDL_FreeSurface(bomb_Sur);
	SDL_DestroyTexture(screen_Txt);
	SDL_FreeSurface(screen_Sur);
	SDL_DestroyTexture(bomb_Txt);
	SDL_DestroyTexture(fruit_Txt);
	SDL_FreeSurface(fruit_Sur);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(windows);
	TTF_CloseFont(comic);
	TTF_CloseFont(times);
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
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
			Event(running);
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