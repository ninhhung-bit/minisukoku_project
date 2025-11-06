#include <SDL.h>
#include<iostream>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include "AudioManager.h"
#include "sudoku.h"
#include "cell.h"
#include "ui.h"
#include "Menu.h"
#include "GameState.h"

using namespace std;

const int SCREEN_WIDTH = 850;
const int SCREEN_HEIGHT = 750;


SDL_Window* cua_so_gemm = nullptr; // đổi tên biến
/*SDL_Renderer* renderer = nullptr; // Thêm biến toàn cục cho renderer
TTF_Font* font = nullptr;         // Thêm biến toàn cục cho font*/

vector<Cell> oTrongBang;
vector<Cell> dayso_luachon;
vector<vector<int>> dapAnDayDu;
vector<vector<int>> bangDangChoi;
// tạo bảng hoàn chỉnh, xoá ngẫu nhiên 1 số ô
void khoitao_bang(int gridSize, int kichthuoc , int ocanxoa) {
   
    dapAnDayDu = taoBangDayDu(gridSize);
    bangDangChoi = dapAnDayDu;
    xoaOngaunhien(bangDangChoi, ocanxoa);

    oTrongBang.clear();

    // Tính toán tổng chiều rộng/cao của bảng
    int boardTotalWidth = gridSize * kichthuoc;
    int boardTotalHeight = gridSize * kichthuoc;
    int khoangcach = 5;  // khoảng cách giữa các ô

    // Tính offset để căn giữa theo chiều ngang
    int startX = (SCREEN_WIDTH - boardTotalWidth) / 2;
    // Tính offset để căn giữa theo chiều dọc
    int startY = (SCREEN_HEIGHT - boardTotalHeight) / 2 - 50; // Trừ bớt 50 để dịch lên thêm 1 týy


    for (int i = 0; i < gridSize; i++)
        for (int j = 0; j < gridSize; j++) {
            Cell otrong;
            otrong.row = i;
            otrong.col = j;
            otrong.value = bangDangChoi[i][j];
            otrong.fixed = (otrong.value != 0); // nếu có số thì không cho sửa nữa
            otrong.rect = {
                j * ( kichthuoc + khoangcach) + startX ,
                i * ( kichthuoc + khoangcach) + startY ,
                kichthuoc - khoangcach ,
                kichthuoc - khoangcach
            };
            oTrongBang.push_back(otrong);
        }

    dayso_luachon.clear();

    int kichthuoco_chon = 60;
    int khoangcach_chon = 10;
    int startX_chon = (SCREEN_WIDTH - gridSize * (kichthuoco_chon + khoangcach_chon))/2 ;

    for (int i = 1; i <= gridSize; i++) {
        Cell ch;
        ch.value = i;
        ch.fixed = true;
        ch.rect = { 
            startX_chon + (i-1) * (kichthuoco_chon + khoangcach_chon),
            startY - kichthuoco_chon - 20 , // cao trên bảng sudoku 1 tý
            kichthuoco_chon,
            kichthuoco_chon
        };
        dayso_luachon.push_back(ch);
    }
}


void runSudoku4x4(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, int ocanxoa) {

    khoitao_bang(4,80,ocanxoa);

    bool dangchay = true;
    bool dang_keo = false;
    int so_dangkeo = 0;
    SDL_Point vitri_chuot = { 0,0 };

    const int ACTUAL_WINDOW_WIDTH = 850; 
    const int ACTUAL_WINDOW_HEIGHT = 750;

    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "4x4.png");

    if (backgroundTexture == NULL)
    {
        printf("loi! SDL_image: %s\n", IMG_GetError());
        return;
    }

    // Kích thước các nút dưới
    int buttonWidth = 140; // chiều rộng nút
    int buttonHeight = 45; // chiều cao nút
    int buttonPadding = 30; // Khoảng cách giữa các nút
    int buttonsStartY = ACTUAL_WINDOW_HEIGHT - buttonHeight - 50; //cách đáy màn hình cao lên 1 chút

    int buttonsGroupStartX = (ACTUAL_WINDOW_WIDTH - (buttonWidth * 3) - (buttonPadding * 2)) / 2; // căn giữa

    while (dangchay) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {

            switch (e.type) {
            case SDL_QUIT:

                dangchay = false;
                break;

                // khi ta nhấn chuột
            case SDL_MOUSEBUTTONDOWN:

                vitri_chuot.x = e.button.x;
                vitri_chuot.y = e.button.y;
                // kiểm tra có đang nhấn vào số ko
                for (auto& so : dayso_luachon) {
                    if (SDL_PointInRect(&vitri_chuot, &so.rect)) {
                        dang_keo = true;
                        so_dangkeo = so.value;
                    }
                }
                break;
                // thả chuột thì gán vào ô tương ứnggg
            case SDL_MOUSEBUTTONUP:
                if (dang_keo) {
                    for (auto& cell : oTrongBang) {
                        if (!cell.fixed && SDL_PointInRect(&vitri_chuot, &cell.rect)) {
                            cell.value = so_dangkeo;
                            bangDangChoi[cell.row][cell.col] = so_dangkeo;
                            break;
                        }
                    }
                    dang_keo = false;
                    so_dangkeo = 0;
                }

                if (vitri_chuot.y > buttonsStartY && vitri_chuot.x > buttonsGroupStartX && 
                    vitri_chuot.x < (buttonsGroupStartX + buttonWidth) && vitri_chuot.y < (buttonsStartY + buttonHeight)) {

                    Mix_HaltMusic();

                    if (kiemtrabang_dung(bangDangChoi)) {

                        AudioManager::getInstance().playWin();
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "you winn!!", ":33 đúng rồi đoáa!", cua_so_gemm);
                    }
                    else {
                        AudioManager::getInstance().playLose();
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Oopps!", "Sai rùi bạn eiii :((", cua_so_gemm);
                    }

                    Mix_PlayMusic(AudioManager::getInstance().getGameMusic(), -1); // chạy lại âm thanh 
                }

                // ấn vào reset thì tạo lại bảngg

                if (vitri_chuot.y > buttonsStartY && vitri_chuot.x > (buttonsGroupStartX + buttonWidth + buttonPadding) &&
                    vitri_chuot.x < (buttonsGroupStartX + 2* buttonWidth + buttonPadding) && vitri_chuot.y < (buttonsStartY + buttonHeight)) {
                    khoitao_bang(4,80,6);
                }


                if (vitri_chuot.y > buttonsStartY && vitri_chuot.x > (buttonsGroupStartX + 2*buttonWidth + 2*buttonPadding) &&
                    vitri_chuot.x < (buttonsGroupStartX + 3 * buttonWidth + 2*buttonPadding) && vitri_chuot.y < (buttonsStartY + buttonHeight)) {
                    // Thoát khỏi vòng while -> quay lại menu
                    return;
                }

                break;

                // chuột dii chuyểnn
            case SDL_MOUSEMOTION:
                vitri_chuot.x = e.motion.x;
                vitri_chuot.y = e.motion.y;
                break;
            }

            if (!dangchay) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);
                SDL_RenderPresent(renderer);
                return; // thoát hẳn về menu
            }

        }

       

        // xoáaa màn hình
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // ve background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // vẽ bànn chơii
        vebangg(oTrongBang, dayso_luachon, so_dangkeo, vitri_chuot, dang_keo);

        // vẽ chữ kiểm tra
        SDL_Rect checkBtn = { buttonsGroupStartX, buttonsStartY , buttonWidth, buttonHeight };
        SDL_SetRenderDrawColor(renderer, 200, 255, 200, 255);
        SDL_RenderFillRect(renderer, &checkBtn);
        ve_kitu("Test!", checkBtn, { 0, 0, 0 });

        // vẽ nút playagainn
        SDL_Rect resetBtn = { buttonsGroupStartX + buttonPadding + buttonWidth , buttonsStartY, buttonWidth, buttonHeight };
        SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
        SDL_RenderFillRect(renderer, &resetBtn);
        ve_kitu("Again!", resetBtn, { 0, 0, 0 });

        // back to menu
        SDL_Rect backBtn = { buttonsGroupStartX + (buttonWidth + buttonPadding) * 2, buttonsStartY, buttonWidth, buttonHeight };
        SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
        SDL_RenderFillRect(renderer, &backBtn);
        ve_kitu("Back!", backBtn, { 0, 0, 0 });


        // show out lên mànn
        SDL_RenderPresent(renderer);

        // làm mượt hơn nè
        SDL_Delay(16);
    }
}


void runSudoku9x9(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, int ocanxoa) {

    khoitao_bang(9, 50, ocanxoa);

    bool dangchay = true;
    bool dang_keo = false;
    int so_dangkeo = 0;
    SDL_Point vitri_chuot = { 0,0 };

    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "9x9.png");

    // Phần layout các nút bên dưới không phụ thuộc vào grid nên có thể giữ nguyên
    int buttonWidth = 140;
    int buttonHeight = 45;
    int buttonPadding = 30;
    int buttonsStartY = SCREEN_HEIGHT - buttonHeight - 50;
    int buttonsGroupStartX = (SCREEN_WIDTH - (buttonWidth * 3) - (buttonPadding * 2)) / 2;

    while (dangchay) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:

                dangchay = false;
                break;

            // khi nhấn chuột
            case SDL_MOUSEBUTTONDOWN:

                vitri_chuot.x = e.button.x;
                vitri_chuot.y = e.button.y;
                // kiểm tra vị trí nhấn
                for (auto& so : dayso_luachon) {
                    if (SDL_PointInRect(&vitri_chuot, &so.rect)) {
                        dang_keo = true;
                        so_dangkeo = so.value;
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:

                // khi thả chuột
                if (dang_keo) {
                    for (auto& cell : oTrongBang) {
                        if (!cell.fixed && SDL_PointInRect(&vitri_chuot, &cell.rect)) {
                            cell.value = so_dangkeo;
                            bangDangChoi[cell.row][cell.col] = so_dangkeo;
                            break;
                        }
                    }
                    dang_keo = false;
                    so_dangkeo = 0;
                }

                if (vitri_chuot.y > buttonsStartY && vitri_chuot.x > buttonsGroupStartX &&
                    vitri_chuot.x < (buttonsGroupStartX + buttonWidth) && vitri_chuot.y < (buttonsStartY + buttonHeight)) {
                    if (kiemtrabang_dung(bangDangChoi)) {
                        AudioManager::getInstance().playWin();
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "you winn!!", ":33 đúng rồi đoáa!", cua_so_gemm);
                    }
                    else {
                        AudioManager::getInstance().playLose();
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Oopps!", "Sai rùi bạn eiii :((", cua_so_gemm);
                    }
                }

                if (vitri_chuot.y > buttonsStartY && vitri_chuot.x > (buttonsGroupStartX + buttonWidth + buttonPadding) &&
                    vitri_chuot.x < (buttonsGroupStartX + 2 * buttonWidth + buttonPadding) && vitri_chuot.y < (buttonsStartY + buttonHeight)) {
                    
                   // Gọi lại khoitao_bang cho đúng màn 9x9
                    khoitao_bang(9, 50, 30);
                }

                if (vitri_chuot.y > buttonsStartY && vitri_chuot.x > (buttonsGroupStartX + 2 * buttonWidth + 2 * buttonPadding) &&
                    vitri_chuot.x < (buttonsGroupStartX + 3 * buttonWidth + 2 * buttonPadding) && vitri_chuot.y < (buttonsStartY + buttonHeight)) {
                    return;
                }
                break;

            case SDL_MOUSEMOTION:
                // khi kéo chuột 

                vitri_chuot.x = e.motion.x;
                vitri_chuot.y = e.motion.y;
                break;
            }
        }

        if (!dangchay) {
            return;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // ve background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        vebangg(oTrongBang, dayso_luachon, so_dangkeo, vitri_chuot, dang_keo);

        SDL_Rect checkBtn = { buttonsGroupStartX, buttonsStartY , buttonWidth, buttonHeight };
        SDL_SetRenderDrawColor(renderer, 200, 255, 200, 255);
        SDL_RenderFillRect(renderer, &checkBtn);
        ve_kitu("Test!", checkBtn, { 0, 0, 0 });

        SDL_Rect resetBtn = { buttonsGroupStartX + buttonPadding + buttonWidth , buttonsStartY, buttonWidth, buttonHeight };
        SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
        SDL_RenderFillRect(renderer, &resetBtn);
        ve_kitu("Again!", resetBtn, { 0, 0, 0 });

        SDL_Rect backBtn = { buttonsGroupStartX + (buttonWidth + buttonPadding) * 2, buttonsStartY, buttonWidth, buttonHeight };
        SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
        SDL_RenderFillRect(renderer, &backBtn);
        ve_kitu("Back!", backBtn, { 0, 0, 0 });

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
}


int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    TTF_Init();

    IMG_Init(IMG_INIT_PNG);

    AudioManager& audio = AudioManager::getInstance();
    audio.init();


    SDL_Window* window = SDL_CreateWindow("Mini Sudoku", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 850, 750, 0);
    SDL_Renderer* main_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* main_font = TTF_OpenFont("arialceb.ttf", 32);

    cua_so_gemm = window;
    renderer = main_renderer;
    font = main_font;


    Menu menu(renderer, font);
    GameState state = GameState::MENU_MAIN;
    bool running = true;

    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else {
                state = menu.handleEvent(e, state);
            }
        }

        if (state == GameState::EXIT) {
            running = false;

        } else if (state == GameState::GAME_4x4) {
            runSudoku4x4(renderer, window, font, 6);
            state = GameState::MENU_MAIN; // quay lại menu sau khi chơi xong
        }
        else if (state == GameState::GAME_9x9) {
            runSudoku9x9(renderer, window, font, 30); 
            state = GameState::MENU_MAIN;
        }
       
        menu.render(state); 
        SDL_RenderPresent(renderer); 
        SDL_Delay(16);
           
    }

    audio.cleanup();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
