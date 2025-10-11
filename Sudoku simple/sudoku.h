#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED
#pragma once
#include <vector>

std::vector<std::vector<int>> taoBangDayDu(int size);
void xoaOngaunhien(std::vector<std::vector<int>>& board, int numEmpty);
bool kiemtrabang_dung(const std::vector<std::vector<int>>& board);

#endif