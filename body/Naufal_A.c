#include "../header/config.h"
#include "../header/Rifky.h"
#include "../header/Naufal_A.h"
#include "../header/Naufal_N.h"

#include <stdlib.h>
// typedef struct 

void drawStoredBlocks(Grid *grid) {

    // Gambar semua block yang sudah didimpan didalam grid
    // for (int y = 0; y < GRID_HEIGHT; y++) {
    //     for (int x = 0; x < GRID_WIDTH; x++) {
    //         if (grid->cells[y][x] != 0) {
    //             setfillstyle(SOLID_FILL, grid->cells[y][x]);
    //             bar(grid->x + x * BLOCK_SIZE, grid->y + y * BLOCK_SIZE, 
    //                 grid->x + (x + 1) * BLOCK_SIZE - 2, grid->y + (y + 1) * BLOCK_SIZE - 2);

    //             setcolor(WHITE);
    //             rectangle(grid->x + x * BLOCK_SIZE, grid->y + y * BLOCK_SIZE, 
    //                       grid->x + (x + 1) * BLOCK_SIZE, grid->y + (y + 1) * BLOCK_SIZE);
    //         }
    //     }
    // }
int count = 0;
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

int clearFullRows(Grid *grid) {
    // // // Inisialisasi variabel untuk menghitung jumlah baris yang dihapus
    // // int rowsCleared = 0;

    // // // Loop setiap baris di grid dari atas ke bawah
    // // for (int y = 0; y < GRID_HEIGHT; y++) {
    // //     int full = 1;  // Anggap baris ini penuh (flag)

    // //     // Loop melalui setiap kolom di baris saat ini
    // //     for (int x = 0; x < GRID_WIDTH; x++) {
    // //         // Jika ada sel yang kosong (nilai 0), baris ini tidak penuh
    // //         if (grid->cells[y][x] == 0) {
    // //             full = 0; // Set flag menjadi 0
    // //             break;    // Keluar dari loop kolom
    // //         }
    // //     }

    // //     // Jika baris penuh full(flag), hapus baris tersebut
    // //     if (full) {
    // //         // Geser semua baris di atas baris yang penuh ke bawah
    // //         for (int i = y; i > 0; i--) {
    // //             for (int j = 0; j < GRID_WIDTH; j++) {
    // //                 // Salin nilai dari baris di atas ke baris saat ini
    // //                 grid->cells[i][j] = grid->cells[i - 1][j];
    // //             }
    // //         }

    // //         // Set baris paling atas (baris 0) menjadi kosong (nilai 0)
    // //         for (int j = 0; j < GRID_WIDTH; j++) {
    // //             grid->cells[0][j] = 0;
    // //         }

    // //         // Tambahkan jumlah baris yang dihapus
    // //         rowsCleared++;
    // //     }
    // // }

    // // // Kembalikan jumlah baris yang dihapus
    // // return rowsCleared;

    int rowsCleared = 0;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        int blockCount = 0;
        StoredBlock* current = grid->blocks;

        // Hitung berapa blok yang ada di baris y
        while (current != NULL) {
            if (current->y == y) {
                blockCount++;
            }
            current = current->next;
        }

        if (blockCount == GRID_WIDTH) {
            // Hapus semua blok di baris y
            StoredBlock **indirect = &grid->blocks;
            while (*indirect != NULL) {
                if ((*indirect)->y == y) {
                        StoredBlock* temp = *indirect;
                        *indirect = (*indirect)->next;
                        free(temp);
                        continue; // <- Tambahkan ini agar tidak lanjut ke bawah
                    } else {
                    if ((*indirect)->y < y) {
                        (*indirect)->y += 1;
                    }
                    indirect = &(*indirect)->next;
                }
            }

            rowsCleared++;
        }
    }

    return rowsCleared;
}

void playSoundEffect(const char* soundFile) {
    PlaySound(soundFile, NULL, SND_LOOP | SND_ASYNC);
}

void stopSound() {
    PlaySound(NULL, NULL, 0);
}

int isGameOver(Grid* grid) {
    // for (int x = 0; x < GRID_WIDTH; x++) {
    //     if (grid->cells[0][x] != 0) {
    //         return 1; // Game Over
    //     }
    // }
    // return 0; // Game masih berjalan


    StoredBlock* current = grid->blocks;
    while (current != NULL) {
        if (current->y <= 1) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void drawGameOverScreen(Grid grid, int score) {
    setcolor(WHITE);
    rectangle(grid.x, grid.y, grid.x + grid.width, grid.y + grid.height);
    
    // Tampilkan "GAME OVER"
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
    outtextxy(670, 200, "GAME OVER");

    // Tampilkan skor akhir
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    char scoreText[30];
    sprintf(scoreText, "Final Score: %d", score);
    outtextxy(670, 260, scoreText);

    // Instruksi untuk memasukkan nama
    outtextxy(670, 320, "Enter your name:");

    // Inisialisasi variabel
    char username[20] = "";
    int i = 0;

    // Input Username
    settextjustify(LEFT_TEXT, TOP_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    
    while (1) {
        char key = getch();

        if (key == 13) { // Enter ditekan
            break;
        } else if (key == 8 && i > 0) { // Backspace
            i--;
            username[i] = '\0';
        } else if (i < sizeof(username) - 1 && (key >= 32 && key <= 126)) {
            // Hanya karakter ASCII printable
            username[i++] = key;
            username[i] = '\0';
        }

        // Bersihkan area input
        setfillstyle(SOLID_FILL, BLACK);
        bar(660, 360, 940, 390);
        
        // Tampilkan username saat ini
        outtextxy(660, 360, username);
    }

    // Jika username kosong, isi dengan "Anonymous"
    if (strlen(username) == 0) {
        strcpy(username, "Anonymous");
    }

    // Simpan skor ke file
    saveScoreToFile(username, score);

    // Tampilkan instruksi keluar
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(670, 420, "Press any key to exit...");
    getch();
}

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

void tutorialPage(){

    initwindow(screenWidth, screenHeight, (char*)"Tutorial Bermain Tetris");

    setbkcolor(BLACK);
    cleardevice();

    // Label Hold
    settextstyle(3, HORIZ_DIR, 8);
    setcolor(MAGENTA);
    outtextxy(30, 20, (char*)"Cara Bermain");
    
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

    settextstyle(10, HORIZ_DIR, 2);
    setcolor(WHITE);
    outtextxy(50, 480, (char*)"Tekan ESC untuk kembali ke main menu");

    while(1) {
        char key = getch();
        if (key == 27) { // ESC key
            showMenu();
            break;
        }
    }

    closegraph();


}


    
    
   
 
