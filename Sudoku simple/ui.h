#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED
#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "cell.h"


extern SDL_Renderer* renderer;
extern TTF_Font* font;

void vebangg(const std::vector<Cell>& grid, const std::vector<Cell>& choices,
	int draggingValue, SDL_Point mousePos, bool dragging);
void ve_kitu(const std::string& text, SDL_Rect rect, SDL_Color color);


#endif // UI_H_INCLUDED
