#include <graphics.h>
#include "../header/tetromino.h"
#include "../header/game.h"
#include "../header/grid.h"

// Definisi bentuk Tetromino (koordinat relatif terhadap pusat rotasi)
const Block tetrominoShapes[7][4] = {
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},   // I (Garis)
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}},   // O (Kotak)
    {{-1, 0}, {0, 0}, {1, 0}, {0, 1}},  // T
    {{1, 0}, {0, 0}, {0, 1}, {0, 2}},   // L
    {{-1, 0}, {0, 0}, {0, 1}, {0, 2}},  // J
    {{0, 0}, {1, 0}, {-1, 1}, {0, 1}},  // Z
    {{-1, 0}, {0, 0}, {0, 1}, {1, 1}}   // S
};

// Warna Tetromino
const int tetrominoColors[7] = {CYAN, YELLOW, MAGENTA, LIGHTRED, BLUE, RED, GREEN};

Tetromino createTetromino(int type, int startX, int startY) {
    Tetromino t;
    for (int i = 0; i < 4; i++) {
        t.blocks[i].x = startX + tetrominoShapes[type][i].x;
        t.blocks[i].y = startY + tetrominoShapes[type][i].y;
    }
    t.color = tetrominoColors[type]; // Menentukan warna berdasarkan tipe
    return t;
}

void drawTetromino(Tetromino t) {
    // setcolor(t.color);
    setfillstyle(SOLID_FILL, t.color);
    
    for (int i = 0; i < 4; i++) {
        int x = t.blocks[i].x * BLOCK_SIZE + 220;  // Koordinat X dalam grid
        int y = t.blocks[i].y * BLOCK_SIZE + 50;   // Koordinat Y dalam grid
        
        // Gambar hanya jika sebagian dari blok masuk layar
        if (t.blocks[i].y >= 0) {  
            bar(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
        }
    }
}

void moveTetromino(Tetromino *t, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        t->blocks[i].x += dx;
        t->blocks[i].y += dy;
    }
}

void rotateTetromino(Tetromino *t) {
    int pivotX = t->blocks[1].x; // Titik pusat rotasi
    int pivotY = t->blocks[1].y;

    Block rotatedBlocks[4];

    for (int i = 0; i < 4; i++) {
        int relativeX = t->blocks[i].x - pivotX;
        int relativeY = t->blocks[i].y - pivotY;

        // Rotasi searah jarum jam (90° CW)
        rotatedBlocks[i].x = pivotX - relativeY;
        rotatedBlocks[i].y = pivotY + relativeX;
    }

    // Cek apakah rotasi valid (tidak keluar dari grid)
    for (int i = 0; i < 4; i++) {
        if (rotatedBlocks[i].x < 0 || rotatedBlocks[i].x >= GRID_WIDTH ||
            rotatedBlocks[i].y < 0 || rotatedBlocks[i].y >= GRID_HEIGHT) {
            return; // Batalkan rotasi jika keluar dari batas grid
        }
    }

    // Terapkan rotasi jika valid
    for (int i = 0; i < 4; i++) {
        t->blocks[i] = rotatedBlocks[i];
    }
}

int canMoveDown(Tetromino *t) {
    for (int i = 0; i < 4; i++) {
        if (t->blocks[i].y + 1 >= GRID_HEIGHT) {
            return 0; // Tidak bisa turun jika menyentuh dasar
        }
    }
    return 1; // Bisa turun
}