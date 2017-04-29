#pragma once
#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>

extern Mix_Music *music;
extern Mix_Chunk *eatfruit;

void PlaySound();
void InitSound();
void QuitSound();
void PlayChunk();

#endif // !1
