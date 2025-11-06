#include "ui.h"
#include <string>

using namespace std;

SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

// vẽ văn bản vào vùng hình chữ nhật
void ve_kitu(const string& ki_tu, SDL_Rect rect, SDL_Color mau) {
    if (!font) {
        SDL_Log("Font not loaded!");
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, ki_tu.c_str(), mau);

    if (!surface) {
        SDL_Log("Failed to create surface: %s", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);  // Giải phóng surrface

    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return;
    }

    int textW = 0, textH = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);

    SDL_Rect dstRect = {
        rect.x + (rect.w - textW) / 2,
        rect.y + (rect.h - textH) / 2,
        textW,
        textH
    };

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);

}
// vẽ bảng sudoku và các ô lựa chọn

void vebangg(const vector<Cell>& ke_o, const vector<Cell>& cac_so, int gia_tri_keo, SDL_Point vitri_chuot, bool dang_keo) {
    
    for (auto& cell : ke_o)  {

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &cell.rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &cell.rect);
        if (cell.value != 0)
            ve_kitu(to_string(cell.value), cell.rect, cell.fixed ? SDL_Color{0,0,0} : SDL_Color{0,0,255});
    }

    for (auto& ch : cac_so) {
        SDL_SetRenderDrawColor(renderer, 230, 230, 250, 255);
        SDL_RenderFillRect(renderer, &ch.rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &ch.rect);
        ve_kitu(to_string(ch.value), ch.rect, SDL_Color{0,0,0});
    }
     if (dang_keo && gia_tri_keo != 0) {
        SDL_Rect dragRect = {vitri_chuot.x - 25, vitri_chuot.y - 25, 50, 50};
        ve_kitu(to_string(gia_tri_keo), dragRect, SDL_Color{255,0,0});   // .vẽ bóng mờ khi kéo số
    }


}
