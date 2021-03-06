#ifndef RENDER_H
#define RENDER_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Object.h"
#include "PowerUps.h"
#include "SnakeF.h"
#include "sstream"
#include "iostream"

using namespace std;

extern SDL_Rect snake_Rect[2][90];
extern SDL_Surface *screen_Sur;
extern SDL_Texture *screen_Txt;
extern SDL_Rect fruit_Rect[2];
extern SDL_Rect rock_Rect[3];
extern SDL_Surface *snake_Sur[2][14];
extern SDL_Texture *snake_Txt[2][14];
extern SDL_Surface *bomb_Sur;
extern SDL_Surface *fruit_Sur;
extern SDL_Texture *fruit_Txt;
extern SDL_Color white;
extern SDL_Rect  obstacle_Rect[2][40];
extern SDL_Window *windows;
extern SDL_Renderer *renderer;
extern TTF_Font *times, *comic;
extern SDL_Surface *pause_button;
extern SDL_Surface *play_button;
extern SDL_Texture *play_txt, *pause_txt;
extern SDL_Texture *bomb_Txt;
extern SDL_Texture *rock_Txt;
extern SDL_Surface *rock_Sur;
extern SDL_Texture *rock_Txt;
extern SDL_Surface *rock_Sur;
extern SDL_Texture *background_Txt[3];
extern SDL_Surface *background_Sur[3];
extern SDL_Texture *pause_menu_Txt;
extern SDL_Surface *pause_menu_Sur;
extern SDL_Surface *music_sur[2];
extern SDL_Texture *music_txt[2];
extern SDL_Surface *attention_sur[2];
extern SDL_Texture *attention_txt[2];
extern SDL_Rect Screen;
extern SDL_Surface *gover_sr;
extern SDL_Texture *gover_txt;


void DrawFruit();
void DrawObstacle();
void DrawPauseButton();
void DrawScore();
void DrawScreen();
void DrawSnake();
void DrawPauseMenu();
void LoadGame();
void PrintResult();
void RenderText (SDL_Surface *, SDL_Rect *);
void Comment();
void DrawMuteButton();
void Quit();
void QuitSurface();
void PayAttention();
void LoadIMG();
#endif

