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

void drawHoldPanel(Panel panel) {
    // Gambar kotak Hold
    rectangle(holdBox.x, holdBox.y, holdBox.x + holdBox.width, holdBox.y + holdBox.height);
    char holdText[] = "Hold";
outtextxy(holdBox.x + 30, holdBox.y + 10, holdText);

    // Jika ada tetromino yang di-hold
    if (isHolding) {
        int minX = 999, minY = 999, maxX = -999, maxY = -999;

        // Cari batas blok tetromino
        for (int i = 0; i < 4; i++) {
            int x = heldTetromino.blocks[i].x;
            int y = heldTetromino.blocks[i].y;

            if (x < minX) minX = x;
            if (y < minY) minY = y;
            if (x > maxX) maxX = x;
            if (y > maxY) maxY = y;
        }

        int tetrominoWidth = (maxX - minX + 1) * (BLOCK_SIZE / 2);
        int tetrominoHeight = (maxY - minY + 1) * (BLOCK_SIZE / 2);
        int startX = holdBox.x + (holdBox.width - tetrominoWidth) / 2;
        int startY = holdBox.y + (holdBox.height - tetrominoHeight) / 2;

        for (int i = 0; i < 4; i++) {
            int bx = startX + (heldTetromino.blocks[i].x - minX) * (BLOCK_SIZE / 2);
            int by = startY + (heldTetromino.blocks[i].y - minY) * (BLOCK_SIZE / 2);

            setfillstyle(SOLID_FILL, heldTetromino.color);
            bar(bx, by, bx + BLOCK_SIZE / 2, by + BLOCK_SIZE / 2);
        }
    }
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
    // gambar panel
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    
    // Tampilkan Teks Score
    char scoreText[20];
    sprintf(scoreText, "Score: %d", *score);
    outtextxy(panel.x + 20, panel.y + 20, scoreText);
    
    
    
    // Semakin tinggi level jatuhnya block semakin cepat
    int levelSpeed = 1;
    if (*score >= 4000) {
        levelSpeed = 4;
    } else if (*score >= 2000) {
        levelSpeed = 3;
    } else if (*score >= 1000) {
        levelSpeed = 2; 
    }
    // Tampilkan teks level
    char speedText[] = "Speed";
    char levelSpeedText[10];
    sprintf(levelSpeedText, "Speed: %d", levelSpeed); 
    outtextxy(panel.x + 20, panel.y + 80, levelSpeedText);
    
    
    
    
    // Tampilkan teks next
    char nextText[] = "Next";
    outtextxy(panel.x + 20, panel.y + 150, nextText);
    // Gambar next block
    drawNextTetromino(nextTetromino, panel.x - 60, panel.y + 240);
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

        // Tulis judul "TETROMANIA" di tengah atas layar
        settextstyle(10, HORIZ_DIR, 5); // Ukuran font besar
        const char* title = "TETROMANIA";

        // Hitung lebar tulisan untuk dipusatkan secara horizontal
        int textWidth = textwidth((char*)title);
        int textHeight = textheight((char*)title);

        // Cetak tulisan di tengah layar (horizontal dan vertikal)
        outtextxy(centerX - textWidth / 2, centerY - 200, (char*)title);

        // Menu
        settextstyle(10, HORIZ_DIR, 3);
        for (int i = 0; i < optionCount; i++) {
            char buffer[100];
            if (i == selectedOption)
                sprintf(buffer, "-> %d. %s", i + 1, options[i]);
            else
                sprintf(buffer, "   %d. %s", i + 1, options[i]);

            outtextxy(centerX - 100, centerY - 50 + i * 50, buffer);
        }

        char key = getch();
        if (key == 72) {
            selectedOption = (selectedOption - 1 + optionCount) % optionCount;
        } else if (key == 80) {
            selectedOption = (selectedOption + 1) % optionCount;
        } else if (key == 13) {
            menuActive = false;
        }
    }

    switch (selectedOption) {
        case 0: playGame(); break;
        case 1: {
            cleardevice();
            Panel leaderboardPanel = {screenWidth / 2 - 200, screenHeight / 2 - 200, 400, 400};
            drawLeadPanel(leaderboardPanel);
            outtextxy(screenWidth / 2 - 150, screenHeight / 2 + 250, (char*)"Press any key to return...");
            getch();
            showMenu(); break;
        }
        case 2: closegraph(); exit(0); break;
    }
}