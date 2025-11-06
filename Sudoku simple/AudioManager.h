#pragma once
#include <SDL_mixer.h>
#include <string>

class AudioManager {
public:
    static AudioManager& getInstance();

    bool init();
    int getVolume() const { return currentVolume; }
    void playMusic(const std::string& path);
    void playClick();
    void playWin();
    void playLose();
    void setVolume(int volume);
    void cleanup();
    Mix_Music* getGameMusic();

private:
    AudioManager() {}
    ~AudioManager() {}
    Mix_Music* bgMusic = nullptr;
    Mix_Chunk* clickSound = nullptr;
    Mix_Chunk* winSound = nullptr;
    Mix_Chunk* loseSound = nullptr;
    int currentVolume = MIX_MAX_VOLUME;
};