#ifndef POWERUPS_H
#define POWERUPS_H

#include "SnakeF.h"
#include "Object.h"
#include "cstdlib"
#include "SDL.h"
extern bool accelerator[2];
extern Uint32 last;
extern Uint32 previous;

void Backto8();
void Continue();
void DecreaseLength();
void DecreaseLengthbyN(int);
void DecreaseLengthbyOne();
void DecreaseLengthbyTwo();
void HalvetheLength();
void IncreaseLength();
void IncreaseLengthbyN(int);
void IncreaseLengthOne();
void IncreaseLengthTwo();
void StoptheSnake();
void Stunning();
void swapLength();
#endif
