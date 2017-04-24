#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Object.h"
#include "PowerUps.h"
#include "SnakeF.h"
#include "sstream"
#include "iostream"
#include "Render.h"
#include "Sound.h"


SDL_Rect snake_Rect[2][90];
SDL_Rect Snake_Box;
SDL_Surface *screen_Sur;
SDL_Texture *screen_Txt;
SDL_Rect fruit_Rect[2];
SDL_Surface *snake_Sur[2][14];
SDL_Texture *snake_Txt[2][14];
SDL_Surface *bomb_Sur;
SDL_Surface *fruit_Sur;
SDL_Texture *fruit_Txt;
SDL_Color white;
SDL_Rect  obstacle_Rect[2][obstacleNum];
SDL_Window *windows;
SDL_Renderer *renderer;
TTF_Font *times, *comic;
SDL_Surface *pause_button;
SDL_Surface *play_button;
SDL_Texture *play_txt, *pause_txt;
SDL_Texture *bomb_Txt;
SDL_Texture *rock_Txt;
SDL_Surface *rock_Sur;
SDL_Rect rock_Rect[3];
SDL_Texture *background_Txt[3];
SDL_Surface *background_Sur[3];
SDL_Texture *pause_menu_Txt;
SDL_Surface *pause_menu_Sur;

int odir[2], direction[2], winner;
int snakeLength[2], snakeLengthRect[2];
int level[2];
bool stop;
bool pause;

void RenderText(SDL_Surface *sur, SDL_Rect *Rect) {
	SDL_Texture *text = NULL;
	text = SDL_CreateTextureFromSurface(renderer, sur);
	if (text == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), windows);
	}
	SDL_RenderCopy(renderer, text, NULL, Rect);
	SDL_DestroyTexture(text);
	SDL_FreeSurface(sur);
}

void DrawObstacle()
{
	
	for (int j = 0; j < 2; ++j)
	{
		int num = int(snakeLength[j] / 3 );
		for (int i = 0; i < 5; ++i)
		{
			if (obstacleState[j][i] == true)
			{
				obstacle_Rect[j][i].x = Obstacles[j][i].x;
				obstacle_Rect[j][i].y = Obstacles[j][i].y;
				obstacle_Rect[j][i].w = Obstacles[j][i].w;
				obstacle_Rect[j][i].h = Obstacles[j][i].h;
				SDL_RenderCopy(renderer, bomb_Txt, NULL, &obstacle_Rect[j][i]);
			}
		}
	}
}

void DrawFruit()
{

	for (int i = 0; i < 2; ++i)
	{
		fruit_Rect[i].x = Fruit[i].x;
		fruit_Rect[i].y = Fruit[i].y;
		fruit_Rect[i].w = Fruit[i].w;
		fruit_Rect[i].h = Fruit[i].h;
		if (fruit_Sur == NULL)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Err", SDL_GetError(), windows);
		}
		SDL_RenderCopy(renderer, fruit_Txt, NULL, &fruit_Rect[i]);
	}
}

void DrawScore()
{
	white = { 255,255,255 };
	stringstream p1score;
	stringstream p2score;
	p1score << snakeLength[1];
	p2score << snakeLength[0];
	SDL_Surface *Player1ScoreNum = TTF_RenderText_Solid(times, p1score.str().c_str(), white);
	SDL_Surface *Player2ScoreNum = TTF_RenderText_Solid(times, p2score.str().c_str(), white);
	SDL_Rect Player2Num_Rect = { 50,280,80,80 };
	SDL_Rect Player1Num_Rect = { 1060,280,80,80 };
	RenderText(Player1ScoreNum, &Player1Num_Rect);
	RenderText(Player2ScoreNum, &Player2Num_Rect);
}

void DrawPauseButton()
{
	SDL_Rect button_img = { window_w / 2 - 25,0,50,50 };
	/*if (pause == false)*/
		pause_button = IMG_Load("./img/pause.png");
	/*else*/
		play_button = IMG_Load("./img/play.png");
	if (pause_button == NULL || play_button == NULL)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Err", SDL_GetError(), windows);
	play_txt = SDL_CreateTextureFromSurface(renderer, play_button);
	pause_txt = SDL_CreateTextureFromSurface(renderer, pause_button);
	if (pause == false)
	{
		SDL_RenderCopy(renderer, pause_txt, NULL, &button_img);
	}
	else
	{
		SDL_RenderCopy(renderer, play_txt, NULL, &button_img);
	}
	SDL_RenderPresent(renderer);
}

void DrawPauseMenu()
{
	SDL_Rect pause_menu_Rect;
	pause_menu_Rect.x = 600 - 150;
	pause_menu_Rect.y = 350 - 155;
	pause_menu_Rect.w = 300;
	pause_menu_Rect.h = 313;
	SDL_RenderCopy(renderer, pause_menu_Txt, NULL, &pause_menu_Rect);
	//SDL_RenderPresent(renderer);
}

void LoadGame()
{
	const int window_w = 1200;
	const int window_h = 700;
	SDL_Init(SDL_INIT_EVERYTHING);
	int video = SDL_VideoInit(NULL);
	TTF_Init();
	InitSound();
	IMG_Init(IMG_INIT_PNG);
	windows = SDL_CreateWindow("Snake Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(windows, -1, SDL_RENDERER_ACCELERATED);
	SetUpSnake();
	SetUpObject();

	times = TTF_OpenFont("./font/times.ttf", 60);
	if (times == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERRR", SDL_GetError(), windows);
	}
	comic = TTF_OpenFont("./font/comic.ttf", 80);
	if (comic == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERRR", SDL_GetError(), windows);
	}
	screen_Sur = IMG_Load("./img/background.png");
	screen_Txt = SDL_CreateTextureFromSurface(renderer, screen_Sur);
	bomb_Sur = IMG_Load("./img/bomb.png");
	bomb_Txt = SDL_CreateTextureFromSurface(renderer, bomb_Sur);
	fruit_Sur = IMG_Load("./img/fruit.png");
	fruit_Txt = SDL_CreateTextureFromSurface(renderer, fruit_Sur);
	rock_Sur = IMG_Load("./img/rock.png");
	rock_Txt = SDL_CreateTextureFromSurface(renderer, rock_Sur);
	pause_menu_Sur = IMG_Load("./img/pausemenu.png");
	pause_menu_Txt = SDL_CreateTextureFromSurface(renderer, pause_menu_Sur);

	snake_Sur[0][0] = IMG_Load("./sna1img/0.png");
	snake_Sur[0][1] = IMG_Load("./sna1img/1.png");
	snake_Sur[0][2] = IMG_Load("./sna1img/2.png");
	snake_Sur[0][3] = IMG_Load("./sna1img/3.png");
	snake_Sur[0][4] = IMG_Load("./sna1img/0duoi.png");
	snake_Sur[0][5] = IMG_Load("./sna1img/1duoi.png");
	snake_Sur[0][6] = IMG_Load("./sna1img/2duoi.png");
	snake_Sur[0][7] = IMG_Load("./sna1img/3duoi.png");
	snake_Sur[0][8] = IMG_Load("./sna1img/1133.png");
	snake_Sur[0][9] = IMG_Load("./sna1img/2200.png");
	snake_Sur[0][10] = IMG_Load("./sna1img/1203.png");
	snake_Sur[0][11] = IMG_Load("./sna1img/2301.png");
	snake_Sur[0][12] = IMG_Load("./sna1img/3021.png");
	snake_Sur[0][13] = IMG_Load("./sna1img/3210.png");

	snake_Sur[1][0] = IMG_Load("./sna2img/0.png");
	snake_Sur[1][1] = IMG_Load("./sna2img/1.png");
	snake_Sur[1][2] = IMG_Load("./sna2img/2.png");
	snake_Sur[1][3] = IMG_Load("./sna2img/3.png");
	snake_Sur[1][4] = IMG_Load("./sna2img/0duoi.png");
	snake_Sur[1][5] = IMG_Load("./sna2img/1duoi.png");
	snake_Sur[1][6] = IMG_Load("./sna2img/2duoi.png");
	snake_Sur[1][7] = IMG_Load("./sna2img/3duoi.png");
	snake_Sur[1][8] = IMG_Load("./sna2img/1133.png");
	snake_Sur[1][9] = IMG_Load("./sna2img/2200.png");
	snake_Sur[1][10] = IMG_Load("./sna2img/1203.png");
	snake_Sur[1][11] = IMG_Load("./sna2img/2301.png");
	snake_Sur[1][12] = IMG_Load("./sna2img/3021.png");
	snake_Sur[1][13] = IMG_Load("./sna2img/3210.png");

	for (int j = 0; j<2; ++j)
		for (int i = 0; i < 14; ++i)
		{
			snake_Txt[j][i] = SDL_CreateTextureFromSurface(renderer, snake_Sur[j][i]);
		}

	background_Sur[0] = IMG_Load("./img/menu_img.png");
	background_Sur[1] = IMG_Load("./img/1win2lose.png");
	background_Sur[2] = IMG_Load("./img/2win1lose.png");
	for (int i = 0; i < 3; ++i)
		background_Txt[i] = SDL_CreateTextureFromSurface(renderer, background_Sur[i]);
}

void PrintResult()
{
	SDL_Rect Screen = { 0,0,1200,700 };
	SDL_Color black = { 0,0,0 };
	SDL_Rect Result_Rect = { Snake_Box.x + Snake_Box.w / 2 - 250, Snake_Box.y + Snake_Box.h / 2 - 40,500,80 };
	SDL_Surface *gameOver = TTF_RenderText_Solid(comic, "Game Over", white);
	RenderText(gameOver, &Result_Rect);
	DrawScore();
	SDL_RenderPresent(renderer);
}

void DrawSnake() {
	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < snakeLength[j]; ++i)
		{
			if (i == 0)
			{
				if (Snake[j][i].img == 1)
				{
					SDL_RenderCopy(renderer, snake_Txt[j][1], NULL, &snake_Rect[j][i]);
				}
				else if (Snake[j][i].img == 0)
				{
					SDL_RenderCopy(renderer, snake_Txt[j][0], NULL, &snake_Rect[j][i]);
				}
				else if (Snake[j][i].img == 2)
				{
					SDL_RenderCopy(renderer, snake_Txt[j][2], NULL, &snake_Rect[j][i]);
				}
				else if (Snake[j][i].img == 3)
				{
					SDL_RenderCopy(renderer, snake_Txt[j][3], NULL, &snake_Rect[j][i]);
				}
			}
			else if (i == snakeLength[j] - 1)
			{
				if (Snake[j][i - 1].img == 1)
				{
					SDL_RenderCopy(renderer, snake_Txt[j][5], NULL, &snake_Rect[j][i]);
				}
				else if (Snake[j][i - 1].img == 0)
				{
					SDL_RenderCopy(renderer, snake_Txt[j][4], NULL, &snake_Rect[j][i]);
				}
				else if (Snake[j][i - 1].img == 2)
				{
					SDL_RenderCopy(renderer, snake_Txt[j][6], NULL, &snake_Rect[j][i]);
				}
				else if (Snake[j][i - 1].img == 3)
				{
					SDL_RenderCopy(renderer, snake_Txt[j][7], NULL, &snake_Rect[j][i]);
				}
			}
			else
			{
				if ((Snake[j][i].img == 1 && Snake[j][i - 1].img == 2) ||
					(Snake[j][i].img == 0 && Snake[j][i - 1].img == 3))
				{
					SDL_RenderCopy(renderer, snake_Txt[j][10], NULL, &snake_Rect[j][i]);
				}
				else if ((Snake[j][i].img == 1 && Snake[j][i - 1].img == 1) ||
					(Snake[j][i].img == 3 && Snake[j][i - 1].img == 3))
				{
					SDL_RenderCopy(renderer, snake_Txt[j][8], NULL, &snake_Rect[j][i]);
				}
				else if ((Snake[j][i].img == 2 && Snake[j][i - 1].img == 2) ||
					(Snake[j][i].img == 0 && Snake[j][i - 1].img == 0))
				{
					SDL_RenderCopy(renderer, snake_Txt[j][9], NULL, &snake_Rect[j][i]);
				}
				else if ((Snake[j][i].img == 3 && Snake[j][i - 1].img == 2) ||
					(Snake[j][i].img == 0 && Snake[j][i - 1].img == 1))
				{
					SDL_RenderCopy(renderer, snake_Txt[j][11], NULL, &snake_Rect[j][i]);
				}
				else if ((Snake[j][i].img == 2 && Snake[j][i - 1].img == 3) ||
					(Snake[j][i].img == 1 && Snake[j][i - 1].img == 0))
				{
					SDL_RenderCopy(renderer, snake_Txt[j][13], NULL, &snake_Rect[j][i]);
				}
				else if ((Snake[j][i].img == 3 && Snake[j][i - 1].img == 0) ||
					(Snake[j][i].img == 2 && Snake[j][i - 1].img == 1))
				{
					SDL_RenderCopy(renderer, snake_Txt[j][12], NULL, &snake_Rect[j][i]);
				}
			}
			//snake_Txt[0][i] = SDL_CreateTextureFromSurface(renderer, snake_Sur[0][i]);
		}
	}
}

void DrawRock()
{
	for (int i = 0; i < 3; ++i)
	{
		rock_Rect[i].x = Rock[i].x;
		rock_Rect[i].y = Rock[i].y;
		rock_Rect[i].w = 40;
		rock_Rect[i].h = 40;
		SDL_RenderCopy(renderer, rock_Txt, NULL, &rock_Rect[i]);
	}
}

void DrawScreen()
{
	//SDL_SetRenderDrawColor(renderer, 227, 229, 129, 158);
	//SDL_RenderClear(renderer);
	SDL_Rect Screen = { 0,0,1200,700 };
	if (stop == false)
	{
		SDL_RenderCopy(renderer, background_Txt[0], NULL, &Screen);
	}
	else
	{
		if (winner == 0)
			SDL_RenderCopy(renderer, background_Txt[1], NULL, &Screen);
		else if (winner == 1)
			SDL_RenderCopy(renderer, background_Txt[2], NULL, &Screen);
	}
	SDL_RenderCopy(renderer, screen_Txt, NULL, &Snake_Box);
	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < snakeLength[j]; ++i)
		{
			if (Snake[j][i].x != 0 && Snake[j][i].y != 0)
			{
				if (i != 0)
				{
					snake_Rect[j][i].x = Snake[j][i].x;
					snake_Rect[j][i].y = Snake[j][i].y;
					snake_Rect[j][i].w = Snake[j][i].w;
					snake_Rect[j][i].h = Snake[j][i].h;
				}
				else
				{
					snake_Rect[j][i].x = Snake[j][i].x - 1;
					snake_Rect[j][i].y = Snake[j][i].y - 1;
					snake_Rect[j][i].w = Snake[j][i].w + 2;
					snake_Rect[j][i].h = Snake[j][i].h + 2;
				}
			}
		}
	}
	DrawRock();
	DrawSnake();
	DrawScore();
	DrawFruit();
	DrawObstacle();
	if (stop == false)
		DrawPauseButton();
	if (stop == true) //stop cho logic, pause cho render
	{
		pause = true;
		PrintResult();
		ContinueGame();
	}
	SDL_RenderPresent(renderer);
}