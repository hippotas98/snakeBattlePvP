#ifndef POWERUPS_H
#define POWERUPS_H

#include "SnakeF.h"
#include "Object.h"
#include "cstdlib"
#include "SDL2/SDL.h"
extern bool swapdirection[2];
extern Uint32 last;
extern Uint32 previous;
extern bool speedup[2];
void Backto5();
void Continue();
void DecreaseLength();
void DecreaseLengthbyN(int);
void DecreaseLengthbyOne();
void DecreaseLengthbyTwo();
void HalvetheLength();
void IncreaseLength();
void increaseLengthbyN(int);
void IncreaseLengthbyOne();
void IncreaseLengthbyTwo();
void StopSwap();
void swapDirection();
void swapLength();

#endif
