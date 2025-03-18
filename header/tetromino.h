#ifndef TETROMINO_H
#define TETROMINO_H

#include "../header/grid.h" // Memastikan Grid dikenali setelah Tetromino dideklarasikan

typedef struct {
    int x, y;
} Block;

typedef struct {
    int x, y;
    Block blocks[4];
    int color;
} Tetromino;

Tetromino createTetromino(int type, int startX, int startY);
void drawTetromino(Tetromino t);
void moveTetromino(Tetromino *t, Grid *grid, int dx, int dy);
void rotateTetromino(Tetromino *t, Grid *grid);
int canMoveDown(Tetromino *t, Grid *grid);
void hardDropTetromino(Tetromino *t, Grid *grid, int *score);
int canMoveTetromino(Tetromino *t, Grid *grid, int dx, int dy);
void storeTetrominoInGrid(Grid *grid, Tetromino *t);
int setRandomTetromino();
int addScore(int *score, Grid *grid);
Tetromino findShadowPosition(Tetromino *t, Grid *grid);
void drawShadowBlock(Tetromino *t, Grid *grid);

#endif