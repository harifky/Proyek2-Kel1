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
    char nextText[] = "Next";
    int levelSpeed = 1;
    

    if (*score >= 4000) {
       levelSpeed = 4;
    } else if (*score >= 2000) {
       levelSpeed = 3;
    } else if (*score >= 1000) {
       levelSpeed = 2; 
    }

    char levelSpeedText[10];
    sprintf(levelSpeedText, "Speed: %d", levelSpeed); 

    
    outtextxy(panel.x + 20, panel.y + 20, scoreText);
    outtextxy(panel.x + 20, panel.y + 50, hiScoreText);
    outtextxy(panel.x + 20, panel.y + 150, nextText);
    outtextxy(panel.x + 20, panel.y + 100, levelSpeedText);
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