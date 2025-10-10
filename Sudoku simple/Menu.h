#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "GameState.h"

struct Button {
    SDL_Rect rect;
    std::string text;
};

class Menu {
public:
    Menu(SDL_Renderer* renderer, TTF_Font* font);

    void render(GameState state);
    GameState handleEvent(SDL_Event& e, GameState state);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;

    std::vector<Button> mainButtons;
    std::vector<Button> difficultyButtons;
    std::vector<Button> settingsButtons;

    void renderButtons(const std::vector<Button>& buttons);
    bool isInside(int x, int y, const SDL_Rect& rect);
};