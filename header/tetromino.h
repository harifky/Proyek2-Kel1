#ifndef TETROMINO_H
#define TETROMINO_H

#include "grid.h"

// Struktur blok individual dalam tetromino
typedef struct {
    int x, y;
} Block;

// Struktur Tetromino
typedef struct {
    int x, y;      // Posisi Tetromino di dalam grid
    Block blocks[4];
    int color;
} Tetromino;

// Fungsi untuk membuat Tetromino
Tetromino createTetromino(int type, int startX, int startY);

// Fungsi untuk menggambar Tetromino
void drawTetromino(Tetromino t);

void moveTetromino(Tetromino *t, Grid *grid, int dx, int dy);

void rotateTetromino(Tetromino *t, Grid *grid);

int canMoveDown(Tetromino *t, Grid *grid);

void hardDropTetromino(Tetromino *t, Grid *grid);

int canMoveTetromino(Tetromino *t, Grid *grid, int dx, int dy);

void storeTetrominoInGrid(Grid *grid, Tetromino *t);

int setRandomTetromino();

#endif