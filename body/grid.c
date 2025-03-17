#include <graphics.h>
#include <string.h>
#include "../header/grid.h"
#include "../header/game.h"
#include "../header/tetromino.h"

Tetromino heldTetromino;
int isHolding = 0;
int hasHeldThisTurn = 0;

// Struktur untuk menyimpan informasi tentang kotak Hold
typedef struct {
    int x, y, width, height;
} HoldBox;

HoldBox holdBox = {100, 50, 100, 100}; // Posisi dan ukuran kotak Hold

// Fungsi untuk menggambar Grid
void drawGrid(Grid grid) {
    for (int i = 0; i <= GRID_WIDTH; i++) {
        line(grid.x + i * BLOCK_SIZE, grid.y, grid.x + i * BLOCK_SIZE, grid.y + GRID_HEIGHT * BLOCK_SIZE);
    }
    for (int i = 0; i <= GRID_HEIGHT; i++) {
        line(grid.x, grid.y + i * BLOCK_SIZE, grid.x + GRID_WIDTH * BLOCK_SIZE, grid.y + i * BLOCK_SIZE);
    }
}

// Fungsi untuk menggambar Hold Panel dan menampilkan tetromino yang di-hold

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


// Fungsi untuk menggambar panel informasi (skor, speed, dll.)
void drawPanel(Panel panel, int *score) {
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

// Fungsi untuk menggambar blok yang sudah tersimpan dalam grid
void drawStoredBlocks(Grid *grid) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid->cells[y][x] != 0) {
                setfillstyle(SOLID_FILL, grid->cells[y][x]);
                bar(grid->x + x * BLOCK_SIZE, grid->y + y * BLOCK_SIZE, 
                    grid->x + (x + 1) * BLOCK_SIZE - 2, grid->y + (y + 1) * BLOCK_SIZE - 2);

                setcolor(WHITE);
                rectangle(grid->x + x * BLOCK_SIZE, grid->y + y * BLOCK_SIZE, 
                          grid->x + (x + 1) * BLOCK_SIZE, grid->y + (y + 1) * BLOCK_SIZE);
            }
        }
    }
}

// Fungsi untuk menghapus baris yang sudah penuh
int clearFullRows(Grid *grid) {
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
            for (int i = y; i > 0; i--) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    grid->cells[i][j] = grid->cells[i - 1][j];
                }
            }

            for (int j = 0; j < GRID_WIDTH; j++) {
                grid->cells[0][j] = 0;
            }

            rowsCleared++; // Tambah jumlah baris yang dihapus
        }
    }

    return rowsCleared;
}
