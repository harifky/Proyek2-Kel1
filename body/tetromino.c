#include <graphics.h>
#include <time.h>
#include "../header/tetromino.h"
#include "../header/game.h"
#include "../header/grid.h"

// Definisi bentuk Tetromino (koordinat relatif terhadap pusat rotasi)
const Block tetrominoShapes[7][4] = {
    {{-1, 0}, {0, 0}, {0, 1}, {0, 2}},   // I (Garis)
    {{1, 0}, {0, 0}, {0, 1}, {1, 1}},   // O (Kotak)
    {{-1, 0}, {0, 0}, {1, 0}, {0, 1}},  // T
    {{-1, 0}, {0, 0}, {1, 0}, {1, 1}},   // L
    {{1, }, {0, 0}, {-1, 0}, {1, -1}},  // J
    {{1, 0}, {0, 0}, {0, 1}, {-1, 1}},  // Z
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
        if (t.blocks[i].y >= -5) {  
            bar(x, y, x + BLOCK_SIZE - 2, y + BLOCK_SIZE - 2);

            // Gambar outline blok
            setcolor(WHITE);  // Ubah warna outline sesuai keinginan
            rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
        }
    }
}

void moveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {
    if (canMoveTetromino(t, grid, dx, dy)) {
        for (int i = 0; i < 4; i++) {
            t->blocks[i].x += dx;
            t->blocks[i].y += dy;
        }
    }
}

int canMoveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        int newX = t->blocks[i].x + dx;
        int newY = t->blocks[i].y + dy;

        // Cek batas grid
        if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT) {
            return 0; // Tidak bisa bergerak
        }

        // Cek tabrakan dengan blok yang sudah tersimpan
        if (newY >= 0 && grid->cells[newY][newX] != 0) {
            return 0; // Tidak bisa bergerak
        }
    }
    return 1; // Bisa bergerak
}

void rotateTetromino(Tetromino *t, Grid *grid) {
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

    // Cek batas grid dan geser jika perlu
    int minX = GRID_WIDTH, maxX = 0;
    for (int i = 0; i < 4; i++) {
        if (rotatedBlocks[i].x < minX) minX = rotatedBlocks[i].x;
        if (rotatedBlocks[i].x > maxX) maxX = rotatedBlocks[i].x;
    }

    int shiftX = 0;
    if (minX < 0) shiftX = -minX;  // Geser ke kanan jika keluar kiri
    if (maxX >= GRID_WIDTH) shiftX = GRID_WIDTH - 1 - maxX; // Geser ke kiri jika keluar kanan

    // Terapkan pergeseran jika diperlukan
    for (int i = 0; i < 4; i++) {
        rotatedBlocks[i].x += shiftX;

        // Cek tabrakan dengan blok lain di grid
        if (rotatedBlocks[i].y >= 0 && grid->cells[rotatedBlocks[i].y][rotatedBlocks[i].x] != 0) {
            return; // Batalkan rotasi jika bertabrakan
        }
    }

    // Terapkan rotasi jika valid
    for (int i = 0; i < 4; i++) {
        t->blocks[i] = rotatedBlocks[i];
    }
}

void hardDropTetromino(Tetromino *t, Grid *grid) {
    while (canMoveDown(t, grid)) {
        moveTetromino(t, grid, 0, 1); // Turunkan terus hingga mentok
    }

    storeTetrominoInGrid(grid, t); // Simpan blok di grid
    clearFullRows(grid); // Hapus baris penuh jika ada

    // Buat Tetromino baru setelah hard drop
    *t = createTetromino(setRandomTetromino(), 5, -2);
}

int canMoveDown(Tetromino *t, Grid *grid) {
    for (int i = 0; i < 4; i++) {
        int x = t->blocks[i].x;
        int y = t->blocks[i].y + 1; // Cek sel di bawahnya

        // Jika sudah di dasar grid, tidak bisa turun
        if (y >= GRID_HEIGHT) {
            return 0; 
        }

        // Cek hanya jika sudah masuk dalam area grid
        if (y >= 0 && grid->cells[y][x] != 0) {
            return 0; // Tidak bisa turun karena ada blok lain
        }
    }
    return 1; // Bisa turun
}

void storeTetrominoInGrid(Grid *grid, Tetromino *t) {
    for (int i = 0; i < 4; i++) {
        int x = t->blocks[i].x;
        int y = t->blocks[i].y;

        if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
            grid->cells[y][x] = t->color;  // Gunakan '.' untuk mengakses struct
        }
    }
}

int setRandomTetromino() {
    srand(time(NULL)); // Seed untuk angka acak
    return rand() % 7; // menentukan jenis tetromino 
}