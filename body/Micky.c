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
    return 1;
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
        moveTetromino(t, grid, 0, 1);
    }
    storeTetrominoInGrid(grid, t);
    *score = addScore(score, grid);
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
        if (key == 13) holdTetromino(tetromino);
    }
}

void updateFrameDelay(int *score) {
    if (*score >= 4000) frameDelay = 150;
    else if (*score >= 3000) frameDelay = 300;
    else if (*score >= 2000) frameDelay = 600;
    else if (*score >= 1000) frameDelay = 1200;
    else frameDelay = 2000;
}
