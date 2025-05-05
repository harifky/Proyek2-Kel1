#include "../header/config.h"
#include "../header/Micky.h"
#include "../header/Naufal_A.h"
#include "../header/Naufal_N.h"
#include "../header/Hafizh.h"

void moveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {
    BlockNode *node = t->head;
    while (node != NULL) {
        int newX = node->x + dx;
        int newY = node->y + dy;
        if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT || (newY >= 0 && grid->cells[newY][newX] != 0)) {
            return;
        }
        node = node->next;
    }

    node = t->head;
    while (node != NULL) {
        node->x += dx;
        node->y += dy;
        node = node->next;
    }
}

void rotateTetromino(Tetromino *t, Grid *grid) {
    BlockNode *pivot = t->head;
    if (pivot == NULL || pivot->next == NULL) return;
    pivot = pivot->next; // gunakan blok ke-2 sebagai pivot

    BlockNode *temp = t->head;
    Block rotated[4];
    int i = 0;
    while (temp != NULL && i < 4) {
        int relX = temp->x - pivot->x;
        int relY = temp->y - pivot->y;
        rotated[i].x = pivot->x - relY;
        rotated[i].y = pivot->y + relX;
        temp = temp->next;
        i++;
    }

    int minX = GRID_WIDTH, maxX = 0;
    for (i = 0; i < 4; i++) {
        if (rotated[i].x < minX) minX = rotated[i].x;
        if (rotated[i].x > maxX) maxX = rotated[i].x;
    }
    int shiftX = 0;
    if (minX < 0) shiftX = -minX;
    if (maxX >= GRID_WIDTH) shiftX = GRID_WIDTH - 1 - maxX;

    for (i = 0; i < 4; i++) {
        rotated[i].x += shiftX;
        if (rotated[i].y >= 0 && grid->cells[rotated[i].y][rotated[i].x] != 0) {
            return; // batal jika bertabrakan
        }
    }

    temp = t->head;
    i = 0;
    while (temp != NULL && i < 4) {
        temp->x = rotated[i].x;
        temp->y = rotated[i].y;
        temp = temp->next;
        i++;
    }
}

int canMoveDown(Tetromino *t, Grid *grid) {
    BlockNode *node = t->head;
    while (node != NULL) {
        int x = node->x;
        int y = node->y + 1;
        if (y >= GRID_HEIGHT || (y >= 0 && grid->cells[y][x] != 0)) {
            return 0;
        }
        node = node->next;
    }
    return 1;
}

int canMoveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {
    BlockNode *node = t->head;
    while (node != NULL) {
        int newX = node->x + dx;
        int newY = node->y + dy;
        if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT || (newY >= 0 && grid->cells[newY][newX] != 0)) {
            return 0;
        }
        node = node->next;
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
