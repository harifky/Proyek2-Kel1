#include <graphics.h>
#include <string.h>

#include "../header/grid.h"
#include "../header/game.h"
#include "../header/tetromino.h"

//fungsi untuk menggambar Grid
void drawGrid(Grid grid) {
    for (int i = 0; i <= GRID_WIDTH; i++) {
        line(grid.x + i * BLOCK_SIZE, grid.y, grid.x + i * BLOCK_SIZE, grid.y + GRID_HEIGHT * BLOCK_SIZE);
    }
    for (int i = 0; i <= GRID_HEIGHT; i++) {
        line(grid.x, grid.y + i * BLOCK_SIZE, grid.x + GRID_WIDTH * BLOCK_SIZE, grid.y + i * BLOCK_SIZE);
    }
}

//fungsi untuk menggambar holdPanel
void drawHoldPanel(Panel panel){
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    char holdText[] = "Hold";

    outtextxy(panel.x + 20, panel.y + 20, holdText);
}

//fungsi untuk menggambar panel
void drawPanel(Panel panel, int *score, Tetromino nextTetromino) {
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    
    char scoreText[20];
    sprintf(scoreText, "Score: %d", *score);
    
    char hiScoreText[] = "Hi-Score";
    char speedText[] = "Speed";

    outtextxy(panel.x + 20, panel.y + 20, scoreText);
    outtextxy(panel.x + 20, panel.y + 50, hiScoreText);
    outtextxy(panel.x + 20, panel.y + 100, speedText);

    // Panggil fungsi untuk menampilkan Tetromino berikutnya
    nextPreviewBlock(panel, nextTetromino);
}


void nextPreviewBlock(Panel panel, Tetromino nextTetromino) {
    // Koordinat awal untuk menggambar Next Block di dalam panel
    int startX = panel.x + 20;
    int startY = panel.y + 180;

    // Gambar label "Next"
    char nextText[] = "Next";
    outtextxy(startX, startY - 20, nextText);

    // Gambar Tetromino di dalam panel
    setfillstyle(SOLID_FILL, nextTetromino.color);
    for (int i = 0; i < 4; i++) {
        int x = startX + nextTetromino.blocks[i].x * BLOCK_SIZE;
        int y = startY + nextTetromino.blocks[i].y * BLOCK_SIZE;

        bar(x, y, x + BLOCK_SIZE - 2, y + BLOCK_SIZE - 2);

        // Outline blok
        setcolor(WHITE);
        rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
    }
}


//fungsi untuk menggambar blok yang sudah disimpan dalam grid
void drawStoredBlocks(Grid *grid) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid->cells[y][x] != 0) { // Jika ada blok di grid
                setfillstyle(SOLID_FILL, grid->cells[y][x]); // Pakai warna sesuai grid
                bar(grid->x + x * BLOCK_SIZE, grid->y + y * BLOCK_SIZE, 
                    grid->x + (x + 1) * BLOCK_SIZE - 2, grid->y + (y + 1) * BLOCK_SIZE - 2);

                // Gambar outline blok
                setcolor(WHITE); // Ubah warna outline sesuai keinginan
                rectangle(grid->x + x * BLOCK_SIZE, grid->y + y * BLOCK_SIZE, 
                grid->x + (x + 1) * BLOCK_SIZE, grid->y + (y + 1) * BLOCK_SIZE);
            }
        }
    }
}

//fungsi untuk menghapus baris yang sudah penuh
int clearFullRows(Grid *grid) {
    printf("Menghapus rows\n");
    int rowsCleared = 0;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        int full = 1;  // Anggap penuh
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid->cells[y][x] == 0) {
                full = 0; // Baris ini tidak penuh
                break;
            }
        }

        if (full) {
            printf("Baris penuh ditemukan di y=%d\n", y);
            // Geser semua baris ke bawah
            for (int i = y; i > 0; i--) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    grid->cells[i][j] = grid->cells[i - 1][j];
                }
            }

            // Kosongkan baris paling atas
            for (int j = 0; j < GRID_WIDTH; j++) {
                grid->cells[0][j] = 0;
            }

            rowsCleared++; // Tambah jumlah baris yang dihapus
        }
    }

    return rowsCleared;
}


// void drawScore(Panel panel, int score){
//     rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

//     char scoreGet[20];
//     sprintf(scoreGet, "Great\n+%d", score);

//     outtextxy(panel.x + 100, panel.y + 100, scoreGet);
// }