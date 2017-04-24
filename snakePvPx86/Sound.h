#pragma once
#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>

extern Mix_Music *music;

void PlaySound();
void InitSound();
void QuitSound();


#endif // !1
