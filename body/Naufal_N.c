#include "../header/config.h"
#include "../header/Naufal_N.h"
#include "../header/Hafizh.h"
#include "../header/game.h"
#include "../header/Rifky.h"
#include "../header/Naufal_A.h"

#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tetromino heldTetromino;
HoldBox holdBox = {490, 50, 100, 100};
int isHolding = 0;

void drawGrid(Grid grid) {
    rectangle(grid.x, grid.y, grid.x + grid.width, grid.y + grid.height);
}

void drawHoldPanel(Panel panel) {
    // Background Hold Box
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // Border Hold Box
    setcolor(WHITE);
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // Label Hold
    settextstyle(10, HORIZ_DIR, 1);
    setcolor(LIGHTCYAN);
    char holdText[] = "Hold";
    outtextxy(panel.x + (panel.width - textwidth(holdText)) / 2, panel.y + 5, (char*)holdText);

    // Gambar tetromino di tengah box jika ada yang di-hold
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

void holdTetromino(Tetromino *current) {
    Tetromino temp = *current;

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

// void drawHoldPanel(Panel panel) {
//     // Label Hold
//     settextstyle(10, HORIZ_DIR, 1);
//     setcolor(LIGHTCYAN);
//     outtextxy(panel.x + 30, panel.y - 20, (char*)"Hold");

//     // Kotak Hold
//     setfillstyle(SOLID_FILL, DARKGRAY);
//     bar(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
//     setcolor(WHITE);
//     rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

//     if (isHolding) {
//         // Hitung bounding box tetromino
//         int minX = 999, minY = 999, maxX = -999, maxY = -999;
//         BlockNode* node = heldTetromino.head;

//         while (node) {
//             if (node->x < minX) minX = node->x;
//             if (node->y < minY) minY = node->y;
//             if (node->x > maxX) maxX = node->x;
//             if (node->y > maxY) maxY = node->y;
//             node = node->next;
//         }

//         int tetrominoWidth = (maxX - minX + 1) * (BLOCK_SIZE / 2);
//         int tetrominoHeight = (maxY - minY + 1) * (BLOCK_SIZE / 2);

//         int startX = panel.x + (panel.width - tetrominoWidth) / 2;
//         int startY = panel.y + (panel.height - tetrominoHeight) / 2;

//         node = heldTetromino.head;
//         while (node) {
//             int bx = startX + (node->x - minX) * (BLOCK_SIZE / 2);
//             int by = startY + (node->y - minY) * (BLOCK_SIZE / 2);

//             setcolor(WHITE);
//             setfillstyle(SOLID_FILL, heldTetromino.color);
//             bar(bx, by, bx + BLOCK_SIZE / 2, by + BLOCK_SIZE / 2);
//             rectangle(bx, by, bx + BLOCK_SIZE / 2, by + BLOCK_SIZE / 2);

//             node = node->next;
//         }
//     }
// }

void drawPanel(Panel panel, int *score) {
    // Background Panel
    setfillstyle(SOLID_FILL, BLACK);
    bar(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // Border Panel
    setcolor(WHITE);
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    // Title Panel
    settextstyle(10, HORIZ_DIR, 2);
    setcolor(LIGHTCYAN);
    outtextxy(panel.x + 20, panel.y + 10, (char*)"GAME STATUS");

    // Skor
    settextstyle(10, HORIZ_DIR, 1);
    setcolor(WHITE);
    char scoreText[20];
    sprintf(scoreText, "Score: %d", *score);
    outtextxy(panel.x + 20, panel.y + 50, scoreText);

    // Speed berdasarkan Score
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

    // Next Label
    setcolor(LIGHTCYAN);
    outtextxy(panel.x + 20, panel.y + 120, (char*)"Next");

    // Kotak Next
    int boxW = 160;
    int boxH = 80;
    int boxX = panel.x + (panel.width - boxW) / 2;
    int boxY = panel.y + 140;

    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(boxX, boxY, boxX + boxW, boxY + boxH);
    setcolor(WHITE);
    rectangle(boxX, boxY, boxX + boxW, boxY + boxH);

    // Gambar next tetromino di tengah box
    drawNextTetromino(nextTetromino, boxX, boxY);
}


void showMenu() {
    initwindow(screenWidth, screenHeight, "Tetris Fullscreen", -3, -3);

    bool menuActive = true;
    int selectedOption = 0;
    const char* options[] = {"Play Game", "Leaderboard", "Tutorial", "Exit"};
    const int optionCount = 4;

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
            tutorialPage();
            break;
        case 3:
            closegraph();
            exit(0);
            break;
    }
}