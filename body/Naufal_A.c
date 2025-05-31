#include "../header/config.h"
#include "../header/Rifky.h"
#include "../header/Naufal_A.h"
#include "../header/Naufal_N.h"
#include <stdlib.h>

// Menampilkan semua blok yang tersimpan di grid
void drawStoredBlocks(Grid *grid) {
    StoredBlock* current = grid->blocks;

    while (current != NULL) {
        setfillstyle(SOLID_FILL, current->color);
        bar(grid->x + current->x * BLOCK_SIZE, grid->y + current->y * BLOCK_SIZE,
            grid->x + (current->x + 1) * BLOCK_SIZE - 2, grid->y + (current->y + 1) * BLOCK_SIZE - 2);

        setcolor(WHITE);
        rectangle(grid->x + current->x * BLOCK_SIZE, grid->y + current->y * BLOCK_SIZE,
                  grid->x + (current->x + 1) * BLOCK_SIZE, grid->y + (current->y + 1) * BLOCK_SIZE);

        current = current->next;
    }
}

// Menghapus baris penuh dari grid dan menggeser blok di atasnya
int clearFullRows(Grid *grid) {
    int rowsCleared = 0;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        int blockCount = 0;
        StoredBlock* current = grid->blocks;

        // Hitung jumlah blok dalam baris y
        while (current != NULL) {
            if (current->y == y) blockCount++;
            current = current->next;
        }

        // Jika baris penuh, hapus blok-bloknya
        if (blockCount == GRID_WIDTH) {
            StoredBlock **indirect = &grid->blocks;

            while (*indirect != NULL) {
                if ((*indirect)->y == y) {
                    StoredBlock* temp = *indirect;
                    *indirect = (*indirect)->next;
                    free(temp);
                    continue;  // Penting: lanjut tanpa menggeser pointer
                }

                // Geser blok di atas baris turun 1
                if ((*indirect)->y < y) {
                    (*indirect)->y += 1;
                }
                indirect = &(*indirect)->next;
            }

            rowsCleared++;
        }
    }

    return rowsCleared;
}

// Memainkan efek suara secara asinkron (berulang)
void playSoundEffect(const char* soundFile) {
    PlaySound(soundFile, NULL, SND_LOOP | SND_ASYNC);
}

// Menghentikan suara
void stopSound() {
    PlaySound(NULL, NULL, 0);
}

// Mengecek apakah permainan sudah berakhir (jika ada blok di atas)
int isGameOver(Grid* grid) {
    StoredBlock* current = grid->blocks;

    while (current != NULL) {
        if (current->y <= 1) return 1;
        current = current->next;
    }

    return 0;
}

// Menampilkan layar Game Over dan menyimpan skor
void drawGameOverScreen(Grid grid, int score) {
    setcolor(WHITE);
    rectangle(grid.x, grid.y, grid.x + grid.width, grid.y + grid.height);

    // Tampilkan tulisan "GAME OVER"
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
    outtextxy(670, 200, "GAME OVER");

    // Tampilkan skor akhir
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    char scoreText[30];
    sprintf(scoreText, "Final Score: %d", score);
    outtextxy(670, 260, scoreText);

    // Input username
    outtextxy(670, 320, "Enter your name:");
    char username[20] = "";
    int i = 0;

    while (1) {
        char key = getch();

        if (key == 13) break;  // Enter ditekan
        else if (key == 8 && i > 0) username[--i] = '\0';  // Backspace
        else if (i < sizeof(username) - 1 && key >= 32 && key <= 126) {
            username[i++] = key;
            username[i] = '\0';
        }

        // Update tampilan input
        setfillstyle(SOLID_FILL, BLACK);
        bar(660, 360, 940, 390);
        outtextxy(660, 360, username);
    }

    if (strlen(username) == 0) strcpy(username, "Anonymous");

    // Simpan skor ke file
    saveScoreToFile(username, score);

    // Tampilkan instruksi keluar
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(670, 420, "Press any key to exit...");
    getch();
}

// Menyimpan posisi blok tetromino ke dalam grid
void storeTetrominoInGrid(Grid *grid, Tetromino *t) {
    for (int i = 0; i < 4; i++) {
        int x = t->blocks[i].x;
        int y = t->blocks[i].y;

        if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
            StoredBlock* newBlock = (StoredBlock*)malloc(sizeof(StoredBlock));
            newBlock->x = x;
            newBlock->y = y;
            newBlock->color = t->color;
            newBlock->next = grid->blocks;
            grid->blocks = newBlock;
        }
    }
}

// Menampilkan halaman tutorial permainan
void tutorialPage() {
    initwindow(screenWidth, screenHeight, (char*)"Tutorial Bermain Tetris");

    setbkcolor(BLACK);
    cleardevice();

    // Judul
    settextstyle(3, HORIZ_DIR, 8);
    setcolor(MAGENTA);
    outtextxy(30, 20, (char*)"Cara Bermain");

    // Tujuan permainan
    settextstyle(2, HORIZ_DIR, 8);
    setcolor(LIGHTBLUE);
    outtextxy(40, 100, (char*)"Tujuan Permainan");

    settextstyle(10, HORIZ_DIR, 3);
    setcolor(WHITE);
    outtextxy(50, 133, (char*)"Susun block-block tetromino yang jatuh agar membentuk baris penuh secara horizontal ");
    outtextxy(50, 153, (char*)"untuk mendapatkan skor!!");
    outtextxy(60, 180, (char*)"- Baris penuh akan menghilang dan menambah skor.");
    outtextxy(60, 200, (char*)"- Permainan berakhir jika blok mencapai atas grid.");
    outtextxy(60, 230, (char*)"- Dapatkan skor tertinggi untuk masuk leaderboard!");

    // Kontrol permainan
    settextstyle(2, HORIZ_DIR, 8);
    setcolor(LIGHTBLUE);
    outtextxy(40, 275, (char*)"Kontrol Permainan");

    settextstyle(10, HORIZ_DIR, 2);
    setcolor(WHITE);
    outtextxy(50, 315, (char*)"[Arrow Up]       : Memutar Tetromino (Rotate)");
    outtextxy(50, 345, (char*)"[Arrow Down]     : Mempercepat jatuhnya Tetromino");
    outtextxy(50, 365, (char*)"[Arrow Left]     : Menggeser Tetromino ke kiri");
    outtextxy(50, 385, (char*)"[Arrow Right]    : Menggeser Tetromino ke kanan");
    outtextxy(50, 415, (char*)"[Spasi]          : Menjatuhkan langsung ke bawah (Hard Drop)");
    outtextxy(50, 445, (char*)"[Enter]          : Menahan (Hold) Tetromino aktif");

    // Instruksi keluar
    outtextxy(50, 480, (char*)"Tekan ESC untuk kembali ke main menu");

    // Menunggu ESC
    while (1) {
        if (getch() == 27) {  // ESC
            showMenu();
            break;
        }
    }

    closegraph();
}