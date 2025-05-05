#ifndef MICKY_H
#define MICKY_H

#include "config.h"

void moveTetromino(Tetromino *t, Grid *grid, int dx, int dy);
void rotateTetromino(Tetromino *t, Grid *grid);
int canMoveDown(Tetromino *t, Grid *grid);
int canMoveTetromino(Tetromino *t, Grid *grid, int dx, int dy);
void hardDropTetromino(Tetromino *t, Grid *grid, int *score);
int addScore(int *score, Grid *grid);
void handleInput(Tetromino *tetromino, Grid *grid, int *score);
void updateFrameDelay(int *score);

#endif