#include "../header/config.h"
#include "../header/Micky.h"
#include "../header/Naufal_A.h"
#include "../header/Naufal_N.h"
#include "../header/Hafizh.h"

// Menggerakkan tetromino ke kiri/kanan/bawah jika tidak bertabrakan
void moveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        int newX = t->blocks[i].x + dx;
        int newY = t->blocks[i].y + dy;

        // Batas grid
        if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT) return;

        // Cek tabrakan dengan blok yang sudah ada
        StoredBlock* current = grid->blocks;
        while (current != NULL) {
            if (current->x == newX && current->y == newY) return;
            current = current->next;
        }
    }

    // Geser posisi tetromino
    for (int i = 0; i < 4; i++) {
        t->blocks[i].x += dx;
        t->blocks[i].y += dy;
    }
}

// Memutar tetromino searah jarum jam dengan pivot di blok ke-2
void rotateTetromino(Tetromino *t, Grid *grid) {
    Block pivot = t->blocks[1]; // Gunakan blok ke-2 sebagai pusat rotasi
    Block rotated[4];

    // Hitung posisi setelah rotasi
    for (int i = 0; i < 4; i++) {
        int relX = t->blocks[i].x - pivot.x;
        int relY = t->blocks[i].y - pivot.y;
        rotated[i].x = pivot.x - relY;
        rotated[i].y = pivot.y + relX;
    }

    // Hitung batas kiri/kanan
    int minX = GRID_WIDTH, maxX = 0;
    for (int i = 0; i < 4; i++) {
        if (rotated[i].x < minX) minX = rotated[i].x;
        if (rotated[i].x > maxX) maxX = rotated[i].x;
    }

    // Geser rotasi agar tetap di dalam grid
    int shiftX = 0;
    if (minX < 0) shiftX = -minX;
    if (maxX >= GRID_WIDTH) shiftX = GRID_WIDTH - 1 - maxX;

    for (int i = 0; i < 4; i++) {
        rotated[i].x += shiftX;

        // Jika tabrakan dengan blok lain, batalkan rotasi
        if (rotated[i].y >= 0) {
            StoredBlock* current = grid->blocks;
            while (current != NULL) {
                if (current->x == rotated[i].x && current->y == rotated[i].y) return;
                current = current->next;
            }
        }
    }

    // Terapkan posisi rotasi jika tidak tabrakan
    for (int i = 0; i < 4; i++) {
        t->blocks[i] = rotated[i];
    }
}

// Mengecek apakah tetromino bisa turun ke bawah
int canMoveDown(Tetromino *t, Grid *grid) {
    for (int i = 0; i < 4; i++) {
        int x = t->blocks[i].x;
        int y = t->blocks[i].y + 1;

        if (y >= GRID_HEIGHT) return 0;

        // Tabrakan dengan blok lain?
        StoredBlock* current = grid->blocks;
        while (current != NULL) {
            if (current->x == x && current->y == y) return 0;
            current = current->next;
        }
    }
    return 1; // Bisa turun
}

// Mengecek apakah tetromino bisa digerakkan ke dx/dy
int canMoveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        int newX = t->blocks[i].x + dx;
        int newY = t->blocks[i].y + dy;

        if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT) return 0;

        // Cek tabrakan dengan blok lain
        StoredBlock* current = grid->blocks;
        while (current != NULL) {
            if (current->x == newX && current->y == newY) return 0;
            current = current->next;
        }
    }
    return 1;
}

// Menambah skor berdasarkan jumlah baris yang dihapus
int addScore(int *score, Grid *grid) {
    int rowsCleared = clearFullRows(grid);
    if (rowsCleared == 1) *score += 100;
    else if (rowsCleared == 2) *score += 250;
    else if (rowsCleared == 3) *score += 400;
    else if (rowsCleared == 4) *score += 800;
    return *score;
}

// Menjatuhkan tetromino langsung ke bawah hingga mentok
void hardDropTetromino(Tetromino *t, Grid *grid, int *score) {
    while (canMoveDown(t, grid)) {
        moveTetromino(t, grid, 0, 1); // Turunkan terus hingga mentok
    }

    storeTetrominoInGrid(grid, t);            // Simpan di grid
    *score = addScore(score, grid);           // Hitung skor
    *t = getNewTetromino();                   // Ambil tetromino baru
}

// Menangani input dari keyboard
void handleInput(Tetromino *tetromino, Grid *grid, int *score) {
    if (kbhit()) {
        char key = getch();
        if (key == 75) moveTetromino(tetromino, grid, -1, 0);  // Kiri
        if (key == 77) moveTetromino(tetromino, grid, 1, 0);   // Kanan
        if (key == 80) moveTetromino(tetromino, grid, 0, 1);   // Bawah
        if (key == 32) hardDropTetromino(tetromino, grid, score); // Spasi (Hard Drop)
        if (key == 72) rotateTetromino(tetromino, grid);       // Atas (Rotate)
        if (key == 13) holdTetromino(tetromino);               // Enter (Hold)
    }
}

// Mempercepat kecepatan jatuh blok berdasarkan skor
void updateFrameDelay(int *score) {
    if (*score >= 4000) frameDelay = 1000 / 60;
    else if (*score >= 3000) frameDelay = 1000 / 40;
    else if (*score >= 2000) frameDelay = 1000 / 30;
    else if (*score >= 1000) frameDelay = 1000 / 20;
    else frameDelay = 1000 / 15;
}
