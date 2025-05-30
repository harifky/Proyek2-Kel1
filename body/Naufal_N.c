#include "../header/config.h"
#include "../header/Naufal_N.h"
#include "../header/Hafizh.h"
#include "../header/game.h"
#include "../header/Rifky.h"

#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tetromino heldTetromino;
HoldBox holdBox = {100, 50, 100, 100};
int isHolding = 0;

void drawGrid(Grid grid) {
    rectangle(grid.x, grid.y, grid.x + grid.width, grid.y + grid.height);
}

void drawHoldPanel(Panel panel){
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    char holdText[] = "Hold";

    outtextxy(panel.x + 20, panel.y + 20, holdText);
}

void holdTetromino(Tetromino *current) {
    Tetromino temp = *current;

    if (!isHolding) {// Jika Hold Box kosong
        heldTetromino = *current;
        isHolding = 1;
        *current = createTetromino(setRandomTetromino(), 5, 0);
    } else {// Jika sudah ada tetromino di Hold Box, tukar
        *current = heldTetromino;
        heldTetromino = temp;
        current->x = 5;
        current->y = 0;
    }
}

void drawPanel(Panel panel, int *score) {
    const int margin = 20;
    const int spacing = 30;
    const int fontSmall = 1;
    const int fontMedium = 2;

    // === Background Panel ===
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // === Border Panel ===
    setcolor(WHITE);
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // === Judul Panel ===
    settextstyle(10, HORIZ_DIR, fontMedium);
    setcolor(LIGHTBLUE);
    outtextxy(panel.x + margin, panel.y + margin, (char*)"STATUS PANEL");

    // === Skor ===
    settextstyle(10, HORIZ_DIR, fontSmall);
    setcolor(WHITE);
    char scoreText[20];
    sprintf(scoreText, "Score: %d", *score);
    outtextxy(panel.x + margin, panel.y + margin + spacing * 2, scoreText);

    // === Level Speed Berdasarkan Skor ===
    int levelSpeed = 1;
    if (*score >= 4000) {
        levelSpeed = 4;
    } else if (*score >= 2000) {
        levelSpeed = 3;
    } else if (*score >= 1000) {
        levelSpeed = 2;
    }

    char levelSpeedText[20];
    sprintf(levelSpeedText, "Speed: %d", levelSpeed);
    outtextxy(panel.x + margin, panel.y + margin + spacing * 3, levelSpeedText);

    // === Next Block Label ===
    setcolor(LIGHTCYAN);
    outtextxy(panel.x + margin, panel.y + margin + spacing * 5, (char*)"Next:");

    // === Next Block Box ===
    int nextBoxTop = panel.y + margin + spacing * 6;
    int nextBoxBottom = nextBoxTop + 80;
    int nextBoxLeft = panel.x + margin;
    int nextBoxRight = panel.x + panel.width - margin;

    // Background kotak
    setfillstyle(SOLID_FILL, BLACK);
    bar(nextBoxLeft, nextBoxTop, nextBoxRight, nextBoxBottom);

    // Border
    setcolor(WHITE);
    rectangle(nextBoxLeft, nextBoxTop, nextBoxRight, nextBoxBottom);

    // Gambar next tetromino di tengah kotak
    int boxW = nextBoxRight - nextBoxLeft;
    int boxH = nextBoxBottom - nextBoxTop;

    drawNextTetromino(nextTetromino, nextBoxLeft, nextBoxTop);

}


void showMenu() {
    initwindow(screenWidth, screenHeight, "Tetris Fullscreen", -3, -3);

    bool menuActive = true;
    int selectedOption = 0;
    const char* options[] = {"Play Game", "Leaderboard", "Exit"};
    const int optionCount = 3;

    while (menuActive) {
        cleardevice();
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;


        // Logo Tetromania //
        // drawTetrisLogo(centerX - 30, 35);  // Sesuaikan posisi

        // === Judul "TETROMANIA" ===
        readimagefile("image/TETROMANIA LOGO.jpg", 360, 30, 900, 180);
        // settextstyle(10, HORIZ_DIR, 6);
        // setcolor(LIGHTBLUE);
        // const char* title = "TETROMANIA";
        // int textWidth = textwidth((char*)title);
        // outtextxy(centerX - textWidth / 2, 80, (char*)title);

        // === Border dan Background Panel Menu ===
        int panelWidth = 400;
        int panelHeight = 300;
        int panelX = centerX - panelWidth / 2;
        int panelY = centerY - panelHeight / 2;

        // Background Panel
        setfillstyle(SOLID_FILL, DARKGRAY);
        bar(panelX, panelY, panelX + panelWidth, panelY + panelHeight);

        // Border Panel
        setcolor(WHITE);
        rectangle(panelX, panelY, panelX + panelWidth, panelY + panelHeight);

        // === Menu Options ===
        settextstyle(10, HORIZ_DIR, 3);
        for (int i = 0; i < optionCount; i++) {
            int optionY = panelY + 50 + i * 60;

            if (i == selectedOption) {
                // Highlight aktif (warna dan blok latar belakang)
                setfillstyle(SOLID_FILL, BLUE);
                bar(panelX + 20, optionY - 10, panelX + panelWidth - 20, optionY + 40);

                setcolor(WHITE);
                char buffer[100];
                sprintf(buffer, "-> %s", options[i]);
                outtextxy(panelX + 40, optionY, buffer);
            } else {
                setcolor(WHITE);
                outtextxy(panelX + 40, optionY, (char*)options[i]);
            }
        }

        // Navigasi menu
        char key = getch();
        if (key == 72) {
            selectedOption = (selectedOption - 1 + optionCount) % optionCount;
        } else if (key == 80) {
            selectedOption = (selectedOption + 1) % optionCount;
        } else if (key == 13) {
            menuActive = false;
        }
    }

    // === Aksi saat menu dipilih ===
    switch (selectedOption) {
        case 0:
            playGame();
            break;
        case 1: {
            cleardevice();
            Panel leaderboardPanel = {screenWidth / 2 - 200, screenHeight / 2 - 200, 400, 400};
            drawLeadPanel(leaderboardPanel);
            outtextxy(screenWidth / 2 - 170, screenHeight / 2 + 230, (char*)"Press any key to return...");
            getch();
            showMenu();
            break;
        }
        case 2:
            closegraph();
            exit(0);
            break;
    }
}