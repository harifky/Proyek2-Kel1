#include "../header/config.h"
#include "../header/Micky.h"
#include "../header/Naufal_A.h"
#include "../header/Naufal_N.h"
#include "../header/Hafizh.h"

void moveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        int newX = t->blocks[i].x + dx;
        int newY = t->blocks[i].y + dy;
        if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT) return;

        StoredBlock* current = grid->blocks;
        while (current != NULL) {
            if (current->x == newX && current->y == newY) return;
            current = current->next;
        }
    }

    for (int i = 0; i < 4; i++) {
        t->blocks[i].x += dx;
        t->blocks[i].y += dy;
    }
}

void rotateTetromino(Tetromino *t, Grid *grid) {
    Block pivot = t->blocks[1]; // gunakan blok ke-2 sebagai pivot
    Block rotated[4];

    for (int i = 0; i < 4; i++) {
        int relX = t->blocks[i].x - pivot.x;
        int relY = t->blocks[i].y - pivot.y;
        rotated[i].x = pivot.x - relY;
        rotated[i].y = pivot.y + relX;
    }

    // Cek batas grid dan geser jika perlu
    int minX = GRID_WIDTH, maxX = 0;
    for (int i = 0; i < 4; i++) {
        if (rotated[i].x < minX) minX = rotated[i].x;
        if (rotated[i].x > maxX) maxX = rotated[i].x;
    }

    int shiftX = 0;
    if (minX < 0) shiftX = -minX;
    if (maxX >= GRID_WIDTH) shiftX = GRID_WIDTH - 1 - maxX;

    for (int i = 0; i < 4; i++) {
        rotated[i].x += shiftX;

        if (rotated[i].y >= 0) {
            StoredBlock* current = grid->blocks;
            while (current != NULL) {
                if (current->x == rotated[i].x && current->y == rotated[i].y) return;
                current = current->next;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        t->blocks[i] = rotated[i];
    }
}

int canMoveDown(Tetromino *t, Grid *grid) {
    for (int i = 0; i < 4; i++) {
        int x = t->blocks[i].x;
        int y = t->blocks[i].y + 1;

        if (y >= GRID_HEIGHT) return 0;

        StoredBlock* current = grid->blocks;
        while (current != NULL) {
            if (current->x == x && current->y == y) return 0;
            current = current->next;
        }
    }
    return 1; // Bisa turun
}

int canMoveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        int newX = t->blocks[i].x + dx;
        int newY = t->blocks[i].y + dy;

        if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT) return 0;

        StoredBlock* current = grid->blocks;
        while (current != NULL) {
            if (current->x == newX && current->y == newY) return 0;
            current = current->next;
        }
    }
    return 1;
}

int addScore(int *score, Grid *grid){
    int rowsCleared = clearFullRows(grid);
    if (rowsCleared == 1) *score += 100;
    else if (rowsCleared == 2) *score += 250;
    else if (rowsCleared == 3) *score += 400;
    else if (rowsCleared == 4) *score += 800;
    return *score;
}

void hardDropTetromino(Tetromino *t, Grid *grid, int *score) {
    while (canMoveDown(t, grid)) {
        moveTetromino(t, grid, 0, 1); // Turunkan terus hingga mentok
    }
    PlaySound("sound/point.wav", NULL, SND_FILENAME | SND_ASYNC);

    storeTetrominoInGrid(grid, t); // Simpan blok di grid
    printf("Memeriksa baris penuh...\n");

    *score = addScore(score, grid);

    // Buat Tetromino baru setelah hard drop
    *t = createTetromino(setRandomTetromino(), 5, -2);
}

void handleInput(Tetromino *tetromino, Grid *grid, int *score) {
    if (kbhit()) {
        char key = getch();
        if (key == 75) moveTetromino(tetromino, grid, -1, 0);
        if (key == 77) moveTetromino(tetromino, grid, 1, 0);
        if (key == 80) moveTetromino(tetromino, grid, 0, 1);
        if (key == 32) hardDropTetromino(tetromino, grid, score);
        if (key == 72) rotateTetromino(tetromino, grid);
        if (key == 13) holdTetromino(tetromino);
    }
}

void updateFrameDelay(int *score) {
    if (*score >= 4000) frameDelay = 1000/60;
    else if (*score >= 3000) frameDelay = 1000/40;
    else if (*score >= 2000) frameDelay = 1000/30;
    else if (*score >= 1000) frameDelay = 1000/20;
    else frameDelay = 1000/15;
}

void drawTetrisLogo(int x, int y) {
    int blockSize = 20; // Ukuran blok
    int padding = 3;    // Jarak antar blok (bisa diabaikan atau diatur)

    // Warna tetromino 'T'
    int color = MAGENTA;
    setfillstyle(SOLID_FILL, color);

    // Bentuk huruf 'T' seperti tetromino T
    int blocks[4][2] = {
        {-1, 0}, {0, 0}, {1, 0}, // baris atas (horizontal)
        {0, 1},                  // blok tengah bawah
    };

    for (int i = 0; i < 5; i++) {
        int dx = blocks[i][0];
        int dy = blocks[i][1];

        int blockX = x + dx * (blockSize + padding);
        int blockY = y + dy * (blockSize + padding);

        bar(blockX, blockY, blockX + blockSize, blockY + blockSize);
        setcolor(WHITE);
        rectangle(blockX, blockY, blockX + blockSize, blockY + blockSize);
    }
}

    