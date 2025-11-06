#pragma once
#include <SDL_mixer.h>

extern Mix_Music* bgMusic;
extern Mix_Chunk* clickSound;

void loadSounds();
void freeSounds();