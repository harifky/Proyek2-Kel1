#ifndef HAFIZH_H
#define HAFIZH_H

#include "config.h" 

Tetromino createTetromino(int type, int startX, int startY);
void drawTetromino(Tetromino t);
void drawNextTetromino(Tetromino next, int boxX, int boxY, int boxWidth, int boxHeight);
Tetromino getNewTetromino();
int setRandomTetromino();
void destroyTetromino(Tetromino* t);

#endif