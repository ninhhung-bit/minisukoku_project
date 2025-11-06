#include "AudioManager.h"
#include <iostream>

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Load các file âm thanh
    bgMusic = Mix_LoadMUS("game.wav");
    clickSound = Mix_LoadWAV("click.wav");
    winSound = Mix_LoadWAV("levelcomplete.wav");
    loseSound = Mix_LoadWAV("gameover.wav");

    if (!bgMusic || !clickSound || !winSound || !loseSound) {
        std::cout << "Không thể load file âm thanh: " << Mix_GetError() << std::endl;
        return false;
    }

    // Bắt đầu nhạc nền
    Mix_PlayMusic(bgMusic, -1);
    return true;
}

void AudioManager::playMusic(const std::string& path) {
    Mix_Music* newMusic = Mix_LoadMUS(path.c_str());
    if (newMusic) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgMusic);
        bgMusic = newMusic;
        Mix_PlayMusic(bgMusic, -1);
    }
}

void AudioManager::playClick() {
    Mix_PlayChannel(-1, clickSound, 0);
}

void AudioManager::playWin() {
    Mix_PlayChannel(-1, winSound, 0);
}

void AudioManager::playLose() {
    Mix_PlayChannel(-1, loseSound, 0);
}

void AudioManager::setVolume(int volume) {
    currentVolume = volume;
    Mix_VolumeMusic(volume);
    Mix_Volume(-1, volume);
}

void AudioManager::cleanup() {
    if (bgMusic) Mix_FreeMusic(bgMusic);
    if (clickSound) Mix_FreeChunk(clickSound);
    if (winSound) Mix_FreeChunk(winSound);
    if (loseSound) Mix_FreeChunk(loseSound);
    Mix_CloseAudio();
    Mix_Quit();
}
Mix_Music* AudioManager::getGameMusic() {
    return bgMusic;
}