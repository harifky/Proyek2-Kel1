#include "../header/Micky.h"
#include "../header/config.h"

void moveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {

    //Block dapat digerakkan jika tidak melebihi batas grid
    if (canMoveTetromino(t, grid, dx, dy)) {
        for (int i = 0; i < 4; i++) {
            t->blocks[i].x += dx;
            t->blocks[i].y += dy;
        }
    }
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

int canMoveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        int newX = t->blocks[i].x + dx;
        int newY = t->blocks[i].y + dy;
        
        // Cek batas grid
        if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT) {
            return 0; // Kembalikan 0 jika Tidak bisa bergerak
        }

        // Cek tabrakan dengan blok yang sudah tersimpan
        if (newY >= 0 && grid->cells[newY][newX] != 0) {
            return 0; // Kembalikan 0 jika Tidak bisa bergerak
        }
    }
    return 1; // Kembalikan 1 jika bisa bergerak
}

int addScore(int *score, Grid *grid){
    // printf("Memeriksa baris penuh...\n"); //? DEBUGGING

    //Mendapatkan jumlah baris yang dihapus
    int rowsCleared = clearFullRows(grid);
    // printf("Baris dihapus: %d\n", rowsCleared); //? DEBUGGING

    // Tambahkan skor berdasarkan jumlah baris yang dihapus
    if (rowsCleared == 1) {
        *score += 100;
    } else if (rowsCleared == 2) { 
        *score += 250;
    } else if (rowsCleared == 3) {
        *score += 400;
    } else if (rowsCleared == 4) {
        *score += 800;
    }

    return *score;

    // printf("Score setelah update: %d\n", *score); //? Debugging
}

void hardDropTetromino(Tetromino *t, Grid *grid, int *score) {
    //block akan langsung dijatuhkan selama condisi canMoveDown)() true
    while (canMoveDown(t, grid)) {
        moveTetromino(t, grid, 0, 1); // Turunkan terus hingga mentok
    }
    
    storeTetrominoInGrid(grid, t); // Simpan blok didalam grid
    // printf("Memeriksa baris penuh...\n"); //?DEBUGGING
    
    *score = addScore(score, grid); //tambahkan score
    
    // Buat Tetromino baru setelah hard drop
    *t = getNewTetromino();
}

void handleInput(Tetromino *tetromino, Grid *grid, int *score) {
    if (kbhit()) {
        char key = getch();
        if (key == 75) moveTetromino(tetromino, grid, -1, 0);
        if (key == 77) moveTetromino(tetromino, grid, 1, 0);
        if (key == 80) moveTetromino(tetromino, grid, 0, 1);
        if (key == 32) hardDropTetromino(tetromino, grid, score);
        if (key == 72) rotateTetromino(tetromino, grid);
        // if (key == 13) holdTetromino(tetromino);
    }
}

void updateFrameDelay(int *score) {
    if (*score >= 4000) {
        frameDelay = 150;  // Sangat cepat
    } else if (*score >= 3000) {
        frameDelay = 300;
    } else if (*score >= 2000) {
        frameDelay = 600;
    } else if (*score >= 1000) {
        frameDelay = 1200;
    } else {
        frameDelay = 2000;  // Kecepatan awal (1 detik per drop)
    }
}