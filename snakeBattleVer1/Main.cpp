#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "cstdlib"
#include "ctime"
#include "cmath"
#include "string"
#include "sstream"
#include "iostream"
using namespace std;

SDL_Rect Snake_Box = { 20,80,705,510 };
SDL_Window *windows;
SDL_Renderer *renderer;
TTF_Font *times, *comic;
const int window_w = 800;
const int window_h = 600;
SDL_Rect Snake1_Rect[100];
SDL_Rect Snake2_Rect[100];
SDL_Color white = { 255,255,255 };
typedef struct
{
	int x, y;
	int ox, oy;
	const int w = 15;
	const int h = 15;
}Snake;
Snake snake1[100];
Snake snake2[100];
int snake1Length, Snake1LengthRect;
int snake2Length, Snake2LengthRect;
const int speed = 15;
typedef struct
{
	int x, y;
	const int w = 15;
	const int h = 15;
}Fruit;
Fruit fruit[2];
SDL_Rect fruit_rect[2];
typedef struct
{
	int x, y;
}Direction;
Direction dir[4];
int odir1, odir2, direction1, direction2, winner;
bool stop, eatfruit;
int fruitbeeaten, level, snakeeatfruit;
int controller1, controller2;
bool accelerator1, accelerator2;
uint32_t last;

//Render text, img
void RenderText(SDL_Surface *sur, SDL_Rect *Rect) {
	SDL_Texture *text = NULL;
	text = SDL_CreateTextureFromSurface(renderer, sur);
	//if (text == NULL)
	//{
	//	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), windows);
	//}
	SDL_RenderCopy(renderer, text, NULL, Rect);
	SDL_DestroyTexture(text);
	SDL_FreeSurface(sur);
}

//Delay 
void Delay() {
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
// Power Up
void increaseLengthbyN(int n)
{
	for (int i = 0; i < n; ++i)
	{
		if (snakeeatfruit == 1)
		{
			snake1[snake1Length].x = snake1[snake1Length - 1].ox;
			snake1[snake1Length].y = snake1[snake1Length - 1].oy;
			snake1Length += 1;
			Snake1LengthRect += 1;
		}
		else if (snakeeatfruit == 2)
		{
			snake2[snake2Length].x = snake2[snake2Length - 1].ox;
			snake2[snake2Length].y = snake2[snake2Length - 1].oy;
			snake2Length += 1;
			Snake2LengthRect += 1;
		}
		if (i == n - 1)
			snakeeatfruit = 3;
	}
}

void DecreaseLengthbyN(int n)
{
	if (snakeeatfruit == 1)
	{
		snake1Length -= n;
		snakeeatfruit = 3;
	}
	else if (snakeeatfruit == 2)
	{
		snake2Length -= n;
		snakeeatfruit = 3;
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

void DoubleLength()
{
	if (snakeeatfruit == 1)
	{
		increaseLengthbyN(snake1Length);
		snakeeatfruit = 3;
	}
	else if (snakeeatfruit == 2)
	{
		increaseLengthbyN(snake2Length);
		snakeeatfruit = 3;
	}

}

void DecreaseLengthbyOne()
{
	DecreaseLengthbyN(1);
}

void DecreaseLengthByTwo()
{
	DecreaseLengthbyN(2);
}

void swapControl()
{
	controller1 = 2 / controller1;
	controller2 = 2 / controller2;
	int temp = snake1Length;
	snake1Length = snake2Length;
	snake2Length = temp;
}

void Backto5()
{
	if (snakeeatfruit == 1)
	{
		int diff = abs(5 - snake1Length);
		if (snake1Length > 5)
			DecreaseLengthbyN(5);
		else if (snake1Length < 5)
			increaseLengthbyN(5);
		snakeeatfruit = 3;
	}
	else if (snakeeatfruit == 2)
	{
		int diff = abs(5 - snake2Length);
		if (snake2Length > 5)
			DecreaseLengthbyN(5);
		else if (snake2Length < 5)
			increaseLengthbyN(5);
		snakeeatfruit = 3;
	}
}

void HalvetheLengh()
{
	if (snakeeatfruit == 1)
	{
		int half = int(snake1Length / 2);
		DecreaseLengthbyN(half);
	}
	else if (snakeeatfruit == 2)
	{
		int half = int(snake2Length / 2);
		DecreaseLengthbyN(half);
	}

}

void Stunning()
{
	if (snakeeatfruit == 1)
	{
		accelerator1 = 0;
		last = SDL_GetTicks();
	}
	else if (snakeeatfruit == 2)
	{
		accelerator2 = 0;
		last = SDL_GetTicks();
	}
}

void Continue()
{
	if (accelerator1 == 0)
		accelerator1 = 1;
	if (accelerator2 == 0)
		accelerator2 = 1;
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

/////// Ham xu li ran
void PowerUpRandom()
{
	int randomnumber = rand() % 11;
	switch (randomnumber)
	{
	case 0: IncreaseLengthbyOne();
		break;
	case 1: IncreaseLengthbyOne();
		break;
	case 2: IncreaseLengthbyTwo();
		break;
	case 3: DoubleLength();
		break;
	case 4: DecreaseLengthbyOne();
		break;
	case 5: DecreaseLengthByTwo();
		break;
	case 6: Backto5();
		break;
	case 7: HalvetheLengh();
		break;
	case 9: swapControl();
		break;
	case 10:Stunning();
		break;
	}
}
////
bool SnakeEatSnake()
{
	for (int i = 0; i < snake1Length; ++i)
	{
		if (snake2[0].x == snake1[i].x  && snake2[0].y == snake1[i].y)
		{
			if (snake1Length >= snake2Length)
			{
				winner = 0;
			}
			else
			{
				winner = 1;
			}
			return true;
		}
	}
	for (int i = 0; i < snake2Length; ++i)
	{
		if (snake1[0].x == snake2[i].x  && snake1[0].y == snake2[i].y)
		{
			if (snake1Length >= snake2Length)
			{
				winner = 0;
			}
			else
			{
				winner = 1;
			}
			return true;
		}
	}
	return false;
}

void SnakeEatFruit()
{
	for (int i = 0; i < 2; ++i)
	{
		if (snake1[0].x == fruit[i].x && snake1[0].y == fruit[i].y)
		{
			eatfruit = true;
			snakeeatfruit = 1;
			fruitbeeaten += 1;
			PowerUpRandom();
		}
		if (snake2[0].x == fruit[i].x && snake2[0].y == fruit[i].y)
		{
			eatfruit = true;
			snakeeatfruit = 2;
			fruitbeeaten += 1;
			PowerUpRandom();
		}
	}
}
//Snake 1

bool Snake1CanMove() {
	if (abs(odir1 - direction1) == 2) return false;
	return true;
}

void snake1Move(int direction)
{
	if (accelerator1 == 1)
	{
		for (int i = 0; i < Snake1LengthRect; ++i)
		{
			if (i == 0)
			{
				snake1[i].ox = snake1[i].x;
				snake1[i].oy = snake1[i].y;
				snake1[i].x += dir[direction].x;
				snake1[i].y += dir[direction].y;
			}
			else
			{
				snake1[i].ox = snake1[i].x;
				snake1[i].oy = snake1[i].y;
				snake1[i].x = snake1[i - 1].ox;
				snake1[i].y = snake1[i - 1].oy;
			}

		}
		Delay();
	}
}

void Controller1Snake1()
{
	const Uint8 *key = SDL_GetKeyboardState(NULL);
	if (Snake1CanMove() == 1)
		odir1 = direction1;
	if (key[SDL_SCANCODE_UP])
	{
		direction1 = 0;
		if (Snake1CanMove() == true)
			snake1Move(direction1);
	}
	else if (key[SDL_SCANCODE_DOWN])
	{
		direction1 = 2;
		if (Snake1CanMove() == true)
			snake1Move(direction1);
	}
	else if (key[SDL_SCANCODE_RIGHT])
	{
		direction1 = 1;
		if (Snake1CanMove() == true)
			snake1Move(direction1);
	}
	else if (key[SDL_SCANCODE_LEFT])
	{
		direction1 = 3;
		if (Snake1CanMove() == true)
			snake1Move(direction1);
	}
}

void Controller2Snake1()
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (Snake1CanMove() == 1)
		odir1 = direction1;
	if (state[SDL_SCANCODE_W])
	{
		direction1 = 0;
		if (Snake1CanMove() == true)
			snake1Move(direction1);
	}
	else if (state[SDL_SCANCODE_S])
	{
		direction1 = 2;
		if (Snake1CanMove() == true)
			snake1Move(direction1);
	}
	else if (state[SDL_SCANCODE_D])
	{
		direction1 = 1;
		if (Snake1CanMove() == true)
			snake1Move(direction1);
	}
	else if (state[SDL_SCANCODE_A])
	{
		direction1 = 3;
		if (Snake1CanMove() == true)
			snake1Move(direction1);
	}
}

void snake1Control()
{
	if (controller1 == 1)
	{
		Controller1Snake1();
	}
	else if (controller1 == 2)
	{
		Controller2Snake1();
	}
}

void snake1Wall()
{
	if (snake1[0].x + snake1[0].w > Snake_Box.x + Snake_Box.w) snake1[0].x = Snake_Box.x;
	else if (snake1[0].x < Snake_Box.x) snake1[0].x = Snake_Box.x + Snake_Box.w - snake1[0].w;
	if (snake1[0].y + snake1[0].h > Snake_Box.y + Snake_Box.h) snake1[0].y = Snake_Box.y;
	else if (snake1[0].y < Snake_Box.y) snake1[0].y = Snake_Box.y + Snake_Box.h - snake1[0].h;
}

bool Snake1Lose()
{
	for (int i = 1; i < snake1Length; ++i)
	{
		if (snake1[0].x == snake1[i].x  && snake1[0].y == snake1[i].y)
		{
			return true;
		}
	}
	if (snake1Length < 2) return true;
	if (snake2Length > 60) return true;
	return false;
}
//Snake 2
bool Snake2CanMove() {
	if (abs(odir2 - direction2) == 2) return false;
	return true;
}

void snake2Move(int direction)
{
	if (accelerator2 == 1)
	{
		for (int i = 0; i < Snake2LengthRect; ++i)
		{
			if (i == 0)
			{
				snake2[0].ox = snake2[0].x;
				snake2[0].oy = snake2[0].y;
				snake2[0].x += (dir[direction].x);
				snake2[0].y += (dir[direction].y);
			}
			else
			{
				snake2[i].ox = snake2[i].x;
				snake2[i].oy = snake2[i].y;
				snake2[i].x = snake2[i - 1].ox;
				snake2[i].y = snake2[i - 1].oy;
			}
		}
		Delay();
	}
}

void Controller2Snake2() {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (Snake2CanMove() == 1)
		odir2 = direction2;
	if (state[SDL_SCANCODE_W])
	{
		direction2 = 0;
		if (Snake2CanMove() == true)
			snake2Move(direction2);
	}
	else if (state[SDL_SCANCODE_S])
	{
		direction2 = 2;
		if (Snake2CanMove() == true)
			snake2Move(direction2);
	}
	else if (state[SDL_SCANCODE_D])
	{
		direction2 = 1;
		if (Snake2CanMove() == true)
			snake2Move(direction2);
	}
	else if (state[SDL_SCANCODE_A])
	{
		direction2 = 3;
		if (Snake2CanMove() == true)
			snake2Move(direction2);
	}
}

void Controller1Snake2()
{
	const Uint8 *key = SDL_GetKeyboardState(NULL);
	if (Snake2CanMove() == 1)
		odir2 = direction2;
	if (key[SDL_SCANCODE_UP])
	{
		direction2 = 0;
		if (Snake2CanMove() == true)
			snake2Move(direction2);
	}
	else if (key[SDL_SCANCODE_DOWN])
	{
		direction2 = 2;
		if (Snake2CanMove() == true)
			snake2Move(direction2);
	}
	else if (key[SDL_SCANCODE_RIGHT])
	{
		direction2 = 1;
		if (Snake2CanMove() == true)
			snake2Move(direction2);
	}
	else if (key[SDL_SCANCODE_LEFT])
	{
		direction2 = 3;
		if (Snake2CanMove() == true)
			snake2Move(direction2);
	}
}

void snake2Control()
{
	if (controller2 == 2)
	{
		Controller2Snake2();
	}
	else if (controller2 == 1)
	{
		Controller1Snake2();
	}
}

void snake2Wall()
{
	if (snake2[0].x + snake2[0].w > Snake_Box.x + Snake_Box.w) snake2[0].x = Snake_Box.x;
	else if (snake2[0].x < Snake_Box.x) snake2[0].x = Snake_Box.x + Snake_Box.w - snake2[0].w;
	if (snake2[0].y + snake2[0].h > Snake_Box.y + Snake_Box.h) snake2[0].y = Snake_Box.y;
	else if (snake2[0].y < Snake_Box.y) snake2[0].y = Snake_Box.y + Snake_Box.h - snake2[0].h;
}

bool Snake2Lose()
{
	for (int i = 1; i < snake2Length; ++i)
	{
		if (snake2[0].x == snake2[i].x  && snake2[0].y == snake2[i].y)
		{
			return true;
		}
	}
	if (snake2Length < 2) return true;
	if (snake1Length > 60) return true;
	return false;
}
// Make Fruits
bool CanPlaceFruitHere(int i)
{
	for (int j = 0; j < snake1Length; ++j)
	{
		if (fruit[i].x == snake1[j].x && fruit[i].y == snake1[j].y)
			return false;
	}
	for (int j = 0; j < snake2Length; ++j)
	{
		if (fruit[i].x == snake2[j].x && fruit[i].y == snake2[j].y)
			return false;
	}
	return true;
}

void MakeFruit()
{
	if (eatfruit == true)
	{
		srand(unsigned int(time(NULL)));
		for (int i = 0; i < 2; ++i)
		{
			for (; ;)
			{
				int x = rand() % 45;
				int y = rand() % 32;
				fruit[i].x = x * 15 + Snake_Box.x + 15;
				fruit[i].y = y * 15 + Snake_Box.y + 15;
				if (CanPlaceFruitHere(i) == true)
					break;
			}
			fruit_rect[i].x = fruit[i].x;
			fruit_rect[i].y = fruit[i].y;
			fruit_rect[i].w = fruit[i].w;
			fruit_rect[i].h = fruit[i].h;
		}
		eatfruit = false;
	}
}
//Draw
void DrawFruit()
{
	SDL_Surface *fruit_img[2];
	/*SDL_SetRenderDrawColor(renderer, 176, 0, 0, 255);
	for (int i = 0; i < 2; ++i)
	{
	SDL_RenderFillRect(renderer, &fruit_rect[i]);
	}*/
	for (int i = 0; i < 2; ++i)
	{
		fruit_img[i] = IMG_Load("fruit.png");
		if (fruit_img[i] == NULL)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Err", SDL_GetError(), windows);
		}
		RenderText(fruit_img[i], &fruit_rect[i]);
	}
}

void DrawScore()
{
	stringstream p1score;
	stringstream p2score;
	p1score << snake1Length;
	p2score << snake2Length;
	SDL_Surface *Player1ScoreNum = TTF_RenderText_Solid(times, p1score.str().c_str(), white);
	SDL_Surface *Player2ScoreNum = TTF_RenderText_Solid(times, p2score.str().c_str(), white);
	SDL_Surface *Player1Text = TTF_RenderText_Solid(times, "Player 1: ", white);
	SDL_Surface *Player2Text = TTF_RenderText_Solid(times, "Player 2: ", white);
	SDL_Rect Player1Text_Rect = { 20,5,90,40 };
	SDL_Rect Player1Num_Rect = { 120,5,30,40 };
	SDL_Rect Player2Text_Rect = { 600,5,90,40 };
	SDL_Rect Player2Num_Rect = { 700,5,30,40 };
	RenderText(Player1Text, &Player1Text_Rect);
	RenderText(Player2Text, &Player2Text_Rect);
	RenderText(Player1ScoreNum, &Player1Num_Rect);
	RenderText(Player2ScoreNum, &Player2Num_Rect);
}

void SetUp()
{
	stop = false;
	winner = 3;
	eatfruit = true;
	snakeeatfruit = 3;
	controller1 = 1;
	controller2 = 2;
	accelerator1 = 1;
	accelerator2 = 1;
	odir1 = 1, odir2 = 3;
	direction1 = 3, direction2 = 1;
	snake1Length = 5;
	snake2Length = 5;
	fruitbeeaten = 0;
	level += 1;
	Snake1LengthRect = 20;
	Snake2LengthRect = 20; // do dai ao, de co kha nang swap do dai

	dir[0].x = 0;
	dir[0].y = -speed;

	dir[1].x = speed;
	dir[1].y = 0;

	dir[2].x = 0;
	dir[2].y = speed;

	dir[3].x = -speed;
	dir[3].y = 0;

	for (int i = 0; i < Snake1LengthRect; ++i)
	{
		if (i >= snake1Length)
		{
			snake1[i].ox = Snake_Box.x + 300 + speed*(snake1Length - i);
			snake1[i].oy = Snake_Box.y + 270;
		}
		else
		{
			snake1[i].x = Snake_Box.x + 300 + speed*(snake1Length - i);
			snake1[i].y = Snake_Box.y + 270;
		}
	}
	for (int i = 0; i < Snake2LengthRect; ++i)
	{
		if (i >= snake1Length)
		{
			snake2[i].ox = Snake_Box.x + speed*i + 15;
			snake2[i].oy = Snake_Box.y + 30;
		}
		else
		{
			snake2[i].x = Snake_Box.x + speed*i + 15;
			snake2[i].y = Snake_Box.y + 30;
		}
	}
}

int GameMenu() {
	Uint32 time;
	int mousex, mousey;
	const int NumofMenu = 2;
	const char *Menu[NumofMenu] = { "Play","Exit" };
	SDL_Surface *MenuSur[NumofMenu];
	SDL_Texture *MenuText[NumofMenu];
	SDL_Rect MenuRect[NumofMenu];
	SDL_Rect Screen = { 0,0,800,600 };
	SDL_Surface *Background;
	SDL_Color red = { 255,0,0 };
	bool Selected[NumofMenu];

	for (int i = 0; i < NumofMenu; ++i) {
		Selected[i] = 0;
	}

	MenuRect[0].x = 350;
	MenuRect[1].x = 360;
	MenuRect[0].y = 320;
	MenuRect[1].y = 400;

	MenuRect[0].h = 60;
	MenuRect[1].h = 60;
	MenuRect[0].w = 100;
	MenuRect[1].w = 80;

	MenuSur[0] = TTF_RenderText_Solid(times, Menu[0], white);
	MenuSur[1] = TTF_RenderText_Solid(times, Menu[1], white);

	IMG_Init(IMG_INIT_PNG);
	Background = IMG_Load("menu.png");
	if (Background == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Err", SDL_GetError(), windows);
	}
	RenderText(Background, &Screen);

	SDL_Rect nameRect = { window_w / 2 - 200,150,400,150 };
	SDL_Surface *nameSurface = TTF_RenderText_Solid(comic, "SNAKE BATTLE", white);
	if (nameSurface == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Err", SDL_GetError(), windows);
	}
	RenderText(nameSurface, &nameRect);

	MenuText[0] = SDL_CreateTextureFromSurface(renderer, MenuSur[0]);
	MenuText[1] = SDL_CreateTextureFromSurface(renderer, MenuSur[1]);
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
							SDL_Surface *Temp = TTF_RenderText_Solid(times, Menu[i], white);
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
			}
		}
	}

	if (30 > (SDL_GetTicks() - time)) {
		SDL_Delay(30 > (SDL_GetTicks() - time));
	}
	return 0;
}

void LoadGame()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	windows = SDL_CreateWindow("Snake Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(windows, -1, SDL_RENDERER_ACCELERATED);
	SetUp();

	times = TTF_OpenFont("times.ttf", 30);
	if (times == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERRR", SDL_GetError(), windows);
	}
	comic = TTF_OpenFont("comic.ttf", 75);
	if (comic == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERRR", SDL_GetError(), windows);
	}

}

void SnakeOne()
{
	snake1Control();
	snake1Wall();
}

void SnakeTwo()
{

	snake2Control();
	snake2Wall();
}
//Tang do kho cho game
void Harder()
{
	if (fruitbeeaten > 5 && fruitbeeaten < 10)
		level = 2;
	else if (fruitbeeaten > 10 && fruitbeeaten < 20)
		level = 3;
	else if (fruitbeeaten > 20 && fruitbeeaten < 30)
		level = 4;
	else if (fruitbeeaten > 30)
		level = 5;
}

void Logic()
{
	MakeFruit();
	SnakeOne();
	SnakeTwo();
	if (Snake2Lose() == true)
	{
		stop = true;
		winner = 0;
	}
	if (Snake1Lose() == true)
	{
		stop = true;
		winner = 1;
	}
	if (SnakeEatSnake() == true)
	{
		stop = true;
	}
	SnakeEatFruit();
	if (accelerator1 == 0 || accelerator2 == 0)
		StoptheSnake();
	Harder();
}

void PrintResult()
{
	SDL_Rect Result_Rect = { Snake_Box.x + Snake_Box.w / 2 - 200, Snake_Box.y + Snake_Box.h / 2 - 40,400,80 };
	if (winner == 0)
	{
		SDL_Surface *Result = TTF_RenderText_Solid(times, "Player 1 WIN. Congratulation !!", white);
		RenderText(Result, &Result_Rect);
	}
	else if (winner == 1)
	{
		SDL_Surface *Result = TTF_RenderText_Solid(times, "Player 2 WIN. Congratulation !!", white);
		RenderText(Result, &Result_Rect);
	}
	DrawScore();
	SDL_RenderPresent(renderer);
}

void Quit();

void ContinueGame()
{
	SDL_Event occur;
	SDL_PollEvent(&occur);
	if (occur.type == SDL_KEYDOWN)
	{
		if (occur.key.keysym.sym == SDLK_SPACE)
		{
			SetUp();
		}
		else if (occur.key.keysym.sym == SDLK_ESCAPE)
		{
			Quit();
		}
	}
}

void DrawScreen()
{
	SDL_SetRenderDrawColor(renderer, 227, 229, 129, 158);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &Snake_Box);
	if (stop == false)
	{
		for (int i = 0; i < snake1Length; ++i)
		{
			if (i == 0)
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}
			if (snake1[i].x != 0 && snake1[i].y != 0)
			{
				Snake1_Rect[i].x = snake1[i].x;
				Snake1_Rect[i].y = snake1[i].y;
				Snake1_Rect[i].w = snake1[i].w;
				Snake1_Rect[i].h = snake1[i].h;
				SDL_RenderFillRect(renderer, &Snake1_Rect[i]);
			}
		}

		for (int i = 0; i < snake2Length; ++i)
		{
			if (i == 0)
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0, 130, 130, 255);
			}
			if (snake2[i].x != 0 && snake2[i].y != 0)
			{
				Snake2_Rect[i].x = snake2[i].x;
				Snake2_Rect[i].y = snake2[i].y;
				Snake2_Rect[i].w = snake2[i].w;
				Snake2_Rect[i].h = snake2[i].h;
				SDL_RenderFillRect(renderer, &Snake2_Rect[i]);
			}
		}
		DrawScore();
		DrawFruit();
		SDL_RenderPresent(renderer);
	}
	else if (stop == true)
	{
		PrintResult();
		ContinueGame();
	}
}

void Quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(windows);
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
}

int main(int argv, char ** args)
{
	bool running = true;
	bool stop = false;
	SDL_Event event;
	LoadGame();
	int menu = GameMenu();
	if (menu == 0)
	{
		while (running)
		{
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT)
			{
				running = false;
				Quit();
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
					Quit();
				}
				if (event.key.keysym.sym == SDLK_p)
				{
					if (stop == false)
						stop = true;
					else if (stop == true)
						stop = false;
				}
			}
			if (stop == false) 
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