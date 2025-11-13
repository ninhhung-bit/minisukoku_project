#include "Menu.h"
#include "GameState.h"
#include "AudioManager.h"
#include <SDL_image.h>
#include <iostream>

using namespace std;


Menu::Menu(SDL_Renderer* renderer, TTF_Font* font) : renderer(renderer), font(font) {

    menuBackgroundTexture = IMG_LoadTexture(renderer, "menu.png");

    // Menu chính
    mainButtons = {
        {{300, 250, 200, 60}, "Level"},
        {{300, 350, 200, 60}, "Setting"},
        {{300, 450, 200, 60}, "Out Game"}
    };
    // Menu độ khó
    difficultyButtons = {
        {{300, 250, 200, 60}, "Easy (4x4)"},
        {{300, 350, 200, 60}, "Hard (9x9)"},
        {{300, 450, 200, 60}, "Back!"}
    };
    // Settings menu
    settingsButtons = {
        {{300, 250, 200, 60}, "Volume"},
        {{300, 350, 200, 60}, "Out game"},
        {{300, 450, 200, 60}, "Back!"}
    };

}

Menu::~Menu() {
    if (menuBackgroundTexture) {
        SDL_DestroyTexture(menuBackgroundTexture);
        menuBackgroundTexture = NULL;
    }
}


void Menu::render(GameState state) {

    if (menuBackgroundTexture) {
        // Vẽ ảnh nền lên toàn bộ màn hình
        SDL_RenderCopy(renderer, menuBackgroundTexture, NULL, NULL);
    }
    else {
        // Nếu ảnh bị lỗi
        SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
        SDL_RenderClear(renderer);
    }

    SDL_Color textColor = { 0, 0, 0, 255 };
    SDL_Surface* titleSurf = TTF_RenderText_Blended(font, "Simple Sudoku", textColor);
    SDL_Texture* titleTex = SDL_CreateTextureFromSurface(renderer, titleSurf);
    int tw, th;
    SDL_QueryTexture(titleTex, NULL, NULL, &tw, &th);
    SDL_Rect titleRect = { 400 - tw / 2, 150, tw, th };
    SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);
    SDL_FreeSurface(titleSurf);
    SDL_DestroyTexture(titleTex);

    if (state == GameState::MENU_MAIN) {
        renderButtons(mainButtons);
    }
    else if (state == GameState::MENU_DIFFICULTY) {
        renderButtons(difficultyButtons);
    }
    else if (state == GameState::MENU_SETTINGS) {
        renderButtons(settingsButtons);
    }
}


void Menu::renderButtons(const std::vector<Button>& buttons) {
    SDL_Color textColor = { 0, 0, 0, 255 };
    for (auto& b : buttons) {
        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255); // nền sáng
        SDL_RenderFillRect(renderer, &b.rect);
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);    // viền đậm   
        SDL_RenderDrawRect(renderer, &b.rect);

        SDL_Color textColor = { 0, 0, 0, 255 };
        SDL_Surface* surf = TTF_RenderText_Blended(font, b.text.c_str(), textColor);
        if (surf) {
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            int tw, th;
            SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
            SDL_Rect dst = { b.rect.x + (b.rect.w - tw) / 2, b.rect.y + (b.rect.h - th) / 2, tw, th };
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
        }
    }
}


bool Menu::isInside(int x, int y, const SDL_Rect& rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
        y >= rect.y && y <= rect.y + rect.h);
}

GameState Menu::handleEvent(SDL_Event& e, GameState state) {

    if (e.type != SDL_MOUSEBUTTONDOWN) {
        return state;
    }
    int mx = e.button.x;
    int my = e.button.y;

    if (state == GameState::MENU_MAIN) {
        if (isInside(mx, my, mainButtons[0].rect)) {
            AudioManager::getInstance().playClick();
            return GameState::MENU_DIFFICULTY;
        }
        if (isInside(mx, my, mainButtons[1].rect)) {
            AudioManager::getInstance().playClick();
            return GameState::MENU_SETTINGS;
        }
        if (isInside(mx, my, mainButtons[2].rect)) {
            AudioManager::getInstance().playClick();
            return GameState::EXIT;
        }
    }
    else if (state == GameState::MENU_DIFFICULTY) {
        if (isInside(mx, my, difficultyButtons[0].rect)) {
            AudioManager::getInstance().playClick();
            return GameState::GAME_4x4;
        }

        if (isInside(mx, my, difficultyButtons[1].rect)) {
            AudioManager::getInstance().playClick();
            return GameState::GAME_9x9;
        }

        if (isInside(mx, my, difficultyButtons[2].rect)) {
            AudioManager::getInstance().playClick();
            return GameState::MENU_MAIN;
        }
    }
    else if (state == GameState::MENU_SETTINGS) {
        if (isInside(mx, my, settingsButtons[0].rect)) {
            AudioManager::getInstance().playClick();

            // âm lượng
            int volume = AudioManager::getInstance().getVolume();   //  lấy âm lượng hiện tại
            volume -= 32;
            if (volume < 0) volume = MIX_MAX_VOLUME;

            AudioManager::getInstance().setVolume(volume);           //  áp dụng lại

            settingsButtons[0].text = "Volume: " + std::to_string(volume);

            cout << "Volume now: " << volume << endl;

        }

        if (isInside(mx, my, settingsButtons[1].rect)) {
            AudioManager::getInstance().playClick();
            return GameState::EXIT;
        }

        if (isInside(mx, my, settingsButtons[2].rect)) {
            AudioManager::getInstance().playClick();
            return GameState::MENU_MAIN;
        }
    }
    return state;
}