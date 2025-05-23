#include "../header/config.h"
#include "../header/Rifky.h"
#include "../header/Naufal_A.h"

// typedef struct 

void drawStoredBlocks(Grid *grid) {

    //Gambar semua block yang sudah didimpan didalam grid
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
    // // Inisialisasi variabel untuk menghitung jumlah baris yang dihapus
    // int rowsCleared = 0;

    // // Loop setiap baris di grid dari atas ke bawah
    // for (int y = 0; y < GRID_HEIGHT; y++) {
    //     int full = 1;  // Anggap baris ini penuh (flag)

    //     // Loop melalui setiap kolom di baris saat ini
    //     for (int x = 0; x < GRID_WIDTH; x++) {
    //         // Jika ada sel yang kosong (nilai 0), baris ini tidak penuh
    //         if (grid->cells[y][x] == 0) {
    //             full = 0; // Set flag menjadi 0
    //             break;    // Keluar dari loop kolom
    //         }
    //     }

    //     // Jika baris penuh full(flag), hapus baris tersebut
    //     if (full) {
    //         // Geser semua baris di atas baris yang penuh ke bawah
    //         for (int i = y; i > 0; i--) {
    //             for (int j = 0; j < GRID_WIDTH; j++) {
    //                 // Salin nilai dari baris di atas ke baris saat ini
    //                 grid->cells[i][j] = grid->cells[i - 1][j];
    //             }
    //         }

    //         // Set baris paling atas (baris 0) menjadi kosong (nilai 0)
    //         for (int j = 0; j < GRID_WIDTH; j++) {
    //             grid->cells[0][j] = 0;
    //         }

    //         // Tambahkan jumlah baris yang dihapus
    //         rowsCleared++;
    //     }
    // }

    // // Kembalikan jumlah baris yang dihapus
    // return rowsCleared;

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
                } else {
                    if ((*indirect)->y < y) {
                        (*indirect)->y += 1; // geser ke bawah
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
        if (current->y == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void drawGameOverScreen(Grid grid, int score) {
    setcolor(WHITE);
    rectangle(grid.x, grid.y, grid.x + grid.width, grid.y + grid.height);
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
    char gameOverText[10] = "GAME OVER";
    outtextxy(275, 200, gameOverText);
    
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    char scoreText[30];
    sprintf(scoreText, "Final Score: %d", score);
    outtextxy(280, 300, scoreText);
    
    // **Minta Username**
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    char username[20];
    char enterNameText[] = "Enter your name:";
    outtextxy(280, 350, enterNameText);

    int i = 0;
    while (1) {
        char key = getch();
        if (key == 13) {
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            char exitText[] = "Press enter to exit...";
            outtextxy(270, 430, exitText);
            break;
        }  // Jika Enter ditekan, selesai
        if (key == 8 && i > 0) { // Jika Backspace ditekan
            i--;
            username[i] = '\0';
        } else if (i < 19) { // Tambahkan karakter ke username
            username[i++] = key;
            username[i] = '\0';
        }

        // Tampilkan input username
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, BLACK);
        bar(280, 370, 480, 400); // Bersihkan area input
        outtextxy(280, 370, username);
    }

    username[i] = '\0'; // Pastikan string diakhiri dengan null terminator

    // **Simpan Skor ke File**
    saveScoreToFile(username, score);
    
    // Tunggu input sebelum keluar
    getch();
}

void storeTetrominoInGrid(Grid *grid, Tetromino *t) {
    // printf("Menyimpan tetromino...\n"); //?DEBUGGING

    //Gambar blok dan simpan jika sudah mencapai batas pergerakan
    // for (int i = 0; i < 4; i++) {
    //     int x = t->blocks[i].x;
    //     int y = t->blocks[i].y;
        
    //     if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
    //         grid->cells[y][x] = t->color;
    //     }
    // }

    BlockNode* current = t->head;
    while (current != NULL) {
        // Only store if within grid bounds
        if (current->y >= 0 && current->x >= 0 && current->x < GRID_WIDTH) {
            StoredBlock* newBlock = (StoredBlock*)malloc(sizeof(StoredBlock));
            newBlock->x = current->x;
            newBlock->y = current->y;
            newBlock->color = t->color;
            newBlock->next = grid->blocks;
            grid->blocks = newBlock;
        }
        current = current->next;
    }

}