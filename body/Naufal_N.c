#include "../header/config.h"
#include "../header/Naufal_N.h"
#include "../header/Hafizh.h"
#include "../header/game.h"
#include "../header/Rifky.h"
#include "../header/Naufal_A.h"

#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Untuk Sleep()

#define DARKGRAY COLOR(64, 64, 64)

// Variabel global untuk hold tetromino dan status hold
Tetromino heldTetromino;
HoldBox holdBox = {490, 50, 100, 100};
int isHolding = 0;

// Menggambar grid permainan sebagai kotak persegi panjang
void drawGrid(Grid grid) {
    rectangle(grid.x, grid.y, grid.x + grid.width, grid.y + grid.height);
}

// Menggambar panel Hold, menampilkan kotak hold dan tetromino yang di-hold (jika ada)
void drawHoldPanel(Panel panel) {
    // Gambar background hold box
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // Gambar border hold box
    setcolor(WHITE);
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // Label "Hold"
    settextstyle(10, HORIZ_DIR, 1);
    setcolor(LIGHTCYAN);
    char holdText[] = "Hold";
    outtextxy(panel.x + (panel.width - textwidth(holdText)) / 2, panel.y + 5, (char*)holdText);

    // Jika ada tetromino yang di-hold, gambar di tengah hold box
    if (isHolding) {
        int minX = 999, minY = 999, maxX = -999, maxY = -999;
        for (int i = 0; i < 4; i++) {
            int x = heldTetromino.blocks[i].x;
            int y = heldTetromino.blocks[i].y;
            if (x < minX) minX = x;
            if (y < minY) minY = y;
            if (x > maxX) maxX = x;
            if (y > maxY) maxY = y;
        }
        int blockSize = BLOCK_SIZE / 2;
        int tetrominoWidth = (maxX - minX + 1) * blockSize;
        int tetrominoHeight = (maxY - minY + 1) * blockSize;
        int startX = panel.x + (panel.width - tetrominoWidth) / 2;
        int startY = panel.y + (panel.height - tetrominoHeight) / 2;

        setcolor(heldTetromino.color);
        setfillstyle(SOLID_FILL, heldTetromino.color);

        for (int i = 0; i < 4; i++) {
            int bx = startX + (heldTetromino.blocks[i].x - minX) * blockSize;
            int by = startY + (heldTetromino.blocks[i].y - minY) * blockSize;
            bar(bx, by, bx + blockSize, by + blockSize);
            rectangle(bx, by, bx + blockSize, by + blockSize);
        }
    }
}

// Fungsi hold tetromino:
// - Jika belum ada yang di-hold, simpan tetromino sekarang ke hold dan ganti dengan next tetromino
// - Jika sudah ada, tukar tetromino saat ini dengan yang di-hold
void holdTetromino(Tetromino *current) {
    if (!isHolding) {
        heldTetromino = *current;
        *current = nextTetromino;
        nextTetromino = getNewTetromino();
        isHolding = 1;
    } else {
        Tetromino temp = *current;
        *current = heldTetromino;
        heldTetromino = temp;
    }
}

// Menggambar panel status permainan yang menampilkan skor, level kecepatan, dan next tetromino
void drawPanel(Panel panel, int *score) {
    // Background panel
    setfillstyle(SOLID_FILL, BLACK);
    bar(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // Border panel
    setcolor(WHITE);
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // Judul panel
    settextstyle(10, HORIZ_DIR, 2);
    setcolor(LIGHTCYAN);
    outtextxy(panel.x + 20, panel.y + 10, (char*)"GAME STATUS");

    // Tampilkan skor
    settextstyle(10, HORIZ_DIR, 1);
    setcolor(WHITE);
    char scoreText[20];
    sprintf(scoreText, "Score: %d", *score);
    outtextxy(panel.x + 20, panel.y + 50, scoreText);

    // Tentukan level kecepatan berdasarkan skor
    int levelSpeed = 1;
    if (*score >= 4000) levelSpeed = 4;
    else if (*score >= 2000) levelSpeed = 3;
    else if (*score >= 1000) levelSpeed = 2;

    char levelSpeedText[20];
    sprintf(levelSpeedText, "Speed: %d", levelSpeed);
    outtextxy(panel.x + 20, panel.y + 80, levelSpeedText);

    // Label next tetromino
    setcolor(LIGHTCYAN);
    outtextxy(panel.x + 20, panel.y + 120, (char*)"Next");

    // Kotak untuk next tetromino
    int boxW = 160, boxH = 80;
    int boxX = panel.x + (panel.width - boxW) / 2;
    int boxY = panel.y + 140;

    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(boxX, boxY, boxX + boxW, boxY + boxH);
    setcolor(WHITE);
    rectangle(boxX, boxY, boxX + boxW, boxY + boxH);

    // Gambar next tetromino di tengah kotak
    drawNextTetromino(nextTetromino, boxX, boxY);
}

// Menampilkan menu utama dan menangani navigasi serta pemilihan menu
void showMenu() {
    initwindow(screenWidth, screenHeight, "Tetris Fullscreen", -3, -3);

    bool menuActive = true;
    int selectedOption = 0;
    const char *options[] = {"Play Game", "Leaderboard", "Tutorial", "Exit"};
    const int optionCount = 4;

    while (menuActive) {
        cleardevice();

        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;

        // Gambar judul "TETROMANIA"
        int tetromaniaWidth = 540;  
        int tetromaniaHeight = 150; 
        int tetromaniaX = centerX - tetromaniaWidth / 2;
        readimagefile("Proyek2-Kel1/image/TETROMANIA.bmp", tetromaniaX, 30,
                      tetromaniaX + tetromaniaWidth, 30 + tetromaniaHeight);

        // Panel menu utama
        int panelWidth = 400;
        int panelHeight = 300;
        int panelX = centerX - panelWidth / 2;
        int panelY = centerY - panelHeight / 2;

        // Background dan border panel
        setfillstyle(SOLID_FILL, DARKGRAY);
        bar(panelX, panelY, panelX + panelWidth, panelY + panelHeight);
        setcolor(WHITE);
        rectangle(panelX, panelY, panelX + panelWidth, panelY + panelHeight);

        // Footer image di bawah panel menu
        int footerWidth = 525;
        int footerHeight = 50;
        int footerX = centerX - footerWidth / 2;
        int footerY = panelY + panelHeight + 50;
        readimagefile("Proyek2-Kel1/image/FOOTER.bmp", footerX, footerY,
                      footerX + footerWidth, footerY + footerHeight);

        // Gambar opsi menu dengan highlight pada opsi yang dipilih
        settextstyle(10, HORIZ_DIR, 3);
        for (int i = 0; i < optionCount; i++) {
            int optionY = panelY + 50 + i * 60;
            if (i == selectedOption) {
                setfillstyle(SOLID_FILL, BLUE);
                bar(panelX + 20, optionY - 10, panelX + panelWidth - 20, optionY + 40);
                setcolor(WHITE);
                char buffer[100];
                sprintf(buffer, "-> %s", options[i]);
                outtextxy(panelX + 40, optionY, buffer);
            } else {
                setcolor(WHITE);
                outtextxy(panelX + 40, optionY, (char *)options[i]);
            }
        }

        // Navigasi menu dengan tombol panah dan enter
        char key = getch();
        if (key == 72) { // Panah atas
            selectedOption = (selectedOption - 1 + optionCount) % optionCount;
        } else if (key == 80) { // Panah bawah
            selectedOption = (selectedOption + 1) % optionCount;
        } else if (key == 13) { // Enter
            menuActive = false;
        }
    }

    // Jalankan aksi berdasarkan pilihan menu
    switch (selectedOption) {
    case 0:
        playGame();
        break;
    case 1: {
        cleardevice();
        Panel leaderboardPanel = {screenWidth / 2 - 200, screenHeight / 2 - 200, 400, 400};
        drawLeadPanel(leaderboardPanel);
        outtextxy(screenWidth / 2 - 170, screenHeight / 2 + 230, (char *)"Press any key to return...");
        getch();
        showMenu();
        break;
    }
    case 2:
        tutorialPage();
        break;
    case 3:
        closegraph();
        exit(0);
        break;
    }
}