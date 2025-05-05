#ifndef HAFIZH_H

#define HAFIZH_H

#include "config.h" 

Tetromino createTetromino(int type, int startX, int startY);
void drawTetromino(Tetromino t);
void drawNextTetromino(Tetromino next, int posX, int posY);
Tetromino getNewTetromino();
int setRandomTetromino();

#endif