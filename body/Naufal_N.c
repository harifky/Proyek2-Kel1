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
        int minX = 5, minY = 5, maxX = -5, maxY = -5;  // Inisialisasi nilai batas
        for (int i = 0; i < 4; i++) {
            if (heldTetromino.blocks[i].x < minX) minX = heldTetromino.blocks[i].x;
            if (heldTetromino.blocks[i].y < minY) minY = heldTetromino.blocks[i].y;
            if (heldTetromino.blocks[i].x > maxX) maxX = heldTetromino.blocks[i].x;
            if (heldTetromino.blocks[i].y > maxY) maxY = heldTetromino.blocks[i].y;
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
    int choice = 0;

    while (menuActive) {
        cleardevice(); // Bersihkan layar

        // Hitung posisi teks agar berada di tengah layar
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;

        settextstyle(10, HORIZ_DIR, 4);
        outtextxy(centerX - 150, centerY - 150, (char*)"TETRIS GAME");

        settextstyle(10, HORIZ_DIR, 3);
        outtextxy(centerX - 100, centerY - 50, (char*)"1. Play Game");
        outtextxy(centerX - 100, centerY, (char*)"2. Leaderboard");
        outtextxy(centerX - 100, centerY + 50, (char*)"3. Exit");
        outtextxy(centerX - 100, centerY + 100, (char*)"Choose (1/2/3):");

        char key = getch();
        if (key == '1') {
            choice = 1;
            menuActive = false;
        } else if (key == '2') {
            choice = 2;
            menuActive = false;
        } else if (key == '3') {
            choice = 3;
            menuActive = false;
        }
    }

    if (choice == 1) {
        playGame(); // Panggil fungsi untuk memulai game
    } else if (choice == 2) {
        cleardevice(); // Bersihkan layar
        Panel leaderboardPanel = {screenWidth / 2 - 200, screenHeight / 2 - 200, 400, 400}; // Panel untuk leaderboard
        drawLeadPanel(leaderboardPanel);
        outtextxy(screenWidth / 2 - 150, screenHeight / 2 + 250, (char*)"Press any key to return...");
        getch();
        showMenu(); // Kembali ke menu setelah melihat leaderboard
    } else if (choice == 3) {
        closegraph(); // Keluar dari program
        exit(0);
    }
}