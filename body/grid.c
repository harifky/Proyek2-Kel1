#include <graphics.h>
#include <string.h>
#include "../header/grid.h"
#include "../header/game.h"
#include "../header/tetromino.h"


void drawGrid(Grid grid) {
    for (int i = 0; i <= GRID_WIDTH; i++) {
        line(grid.x + i * BLOCK_SIZE, grid.y, grid.x + i * BLOCK_SIZE, grid.y + GRID_HEIGHT * BLOCK_SIZE);
    }
    for (int i = 0; i <= GRID_HEIGHT; i++) {
        line(grid.x, grid.y + i * BLOCK_SIZE, grid.x + GRID_WIDTH * BLOCK_SIZE, grid.y + i * BLOCK_SIZE);
    }
}

void drawHoldPanel(Panel panel){
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    char holdText[] = "Hold";

    outtextxy(panel.x + 20, panel.y + 20, holdText);
}

void drawPanel(Panel panel, int score) {
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    char hiScoreText[] = "Hi-Score";
    char speedText[] = "Speed";
    char nextText[] = "Next";

    outtextxy(panel.x + 20, panel.y + 20, scoreText);
    outtextxy(panel.x + 20, panel.y + 50, hiScoreText);
    outtextxy(panel.x + 20, panel.y + 100, speedText);
    outtextxy(panel.x + 20, panel.y + 150, nextText);
}

// Fungsi untuk menggambar ulang blok yang sudah tersimpan di grid
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

int clearFullRows(Grid *grid) {
    int rowsCleared = 0;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        bool full = true;
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid->cells[y][x] == 0) {
                full = false;
                break;
            }
        }

        if (full) {
            rowsCleared++;

            // Geser semua baris di atas ke bawah
            for (int row = y; row > 0; row--) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    grid->cells[row][x] = grid->cells[row - 1][x];
                }
            }

            // Kosongkan baris paling atas
            for (int x = 0; x < GRID_WIDTH; x++) {
                grid->cells[0][x] = 0;
            }
        }
    }

    return rowsCleared; // Kembalikan jumlah baris yang dihapus
}
