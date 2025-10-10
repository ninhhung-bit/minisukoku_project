#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED

#pragma once
#include <SDL.h>

struct Cell {
    int row, col;
    int value;
    bool fixed;
    SDL_Rect rect;
};

#endif // CELL_H_INCLUDED
