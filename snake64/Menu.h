#pragma once
#ifndef MENU_H
#define MENU_H
#include "SDL.h"
extern SDL_Event event;

int GameMenu();
void Pause(bool *);
void PauseGame(bool *);
void Instruction(int *, bool *);
void Music();
bool SelectMute();
bool SelectPause();


#endif // !


