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
        BlockNode* node = heldTetromino.head;

        while (node) {
            if (node->x < minX) minX = node->x;
            if (node->y < minY) minY = node->y;
            if (node->x > maxX) maxX = node->x;
            if (node->y > maxY) maxY = node->y;
            node = node->next;
        }

        int tetrominoWidth = (maxX - minX + 1) * (BLOCK_SIZE / 2);
        int tetrominoHeight = (maxY - minY + 1) * (BLOCK_SIZE / 2);
        int startX = holdBox.x + (holdBox.width - tetrominoWidth) / 2;
        int startY = holdBox.y + (holdBox.height - tetrominoHeight) / 2;

        node = heldTetromino.head;
        while (node) {
            int bx = startX + (node->x - minX) * (BLOCK_SIZE / 2);
            int by = startY + (node->y - minY) * (BLOCK_SIZE / 2);

            setfillstyle(SOLID_FILL, heldTetromino.color);
            bar(bx, by, bx + BLOCK_SIZE / 2, by + BLOCK_SIZE / 2);
            node = node->next;
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
    // === Background Panel ===
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // === Border Panel ===
    setcolor(WHITE);
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // === Judul Panel ===
    settextstyle(10, HORIZ_DIR, 2);
    setcolor(LIGHTBLUE);
    outtextxy(panel.x + 20, panel.y + 10, (char*)"STATUS PANEL");

    // === Skor ===
    settextstyle(10, HORIZ_DIR, 1);
    setcolor(WHITE);
    char scoreText[20];
    sprintf(scoreText, "Score: %d", *score);
    outtextxy(panel.x + 20, panel.y + 50, scoreText);

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
    outtextxy(panel.x + 20, panel.y + 80, levelSpeedText);

    // === Next Block ===
    setcolor(LIGHTCYAN);
    outtextxy(panel.x + 20, panel.y + 130, (char*)"Next:");

    // Kotak latar belakang next block
    setfillstyle(SOLID_FILL, BLACK);
    bar(panel.x + 15, panel.y + 150, panel.x + panel.width - 15, panel.y + 230);

    // Border next block box
    setcolor(WHITE);
    rectangle(panel.x + 15, panel.y + 150, panel.x + panel.width - 15, panel.y + 230);

    // Gambar next tetromino
    int boxX = panel.x + 15;
    int boxY = panel.y + 150;
    int boxW = panel.width - 30;
    int boxH = 80;  // dari 150 ke 230
    
    drawNextTetromino(nextTetromino, boxX, boxY, boxW, boxH);
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

        // **Panggil fungsi drawLogo di sini, misal di kiri atas**
        // drawLogo(centerX - 100, centerY - 1000);  // contoh posisi, sesuaikan

        // Tulis judul "TETROMANIA" di tengah atas layar
        settextstyle(10, HORIZ_DIR, 5); // Ukuran font besar
        const char* title = "TETROMANIA";
        int textWidth = textwidth((char*)title);
        outtextxy(centerX - textWidth / 2, 80, (char*)title);

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