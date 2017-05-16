#include "Sound.h"
#include "SDL2/SDL_mixer.h"
#include "Render.h"

Mix_Music *music;
Mix_Chunk *eatfruit;

void InitSound()
{
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error Music", SDL_GetError(), windows);
	}
	music = Mix_LoadMUS("./sound/themesong.wav");
	if(music == NULL)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error Music", SDL_GetError(), windows);
	eatfruit = Mix_LoadWAV("./sound/eatFruit.wav");
	if (eatfruit == NULL)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error Music", SDL_GetError(), windows);
}

void PlaySound()
{
	if (Mix_PlayingMusic() == 0)
	{
		//Play the music
		if (Mix_PlayMusic(music, -1) == -1)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error Music", SDL_GetError(), windows);
		}
	}
}

void PlayChunk()
{
	if (Mix_PlayChannel(-1, eatfruit, 0) == -1)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error Music", SDL_GetError(), windows);
	}
}

void QuitSound()
{
	Mix_FreeMusic(music);
	Mix_FreeChunk(eatfruit);
	Mix_CloseAudio();
	Mix_Quit();
}
