#ifndef NAUFAL_A_H
#define NAUFAL_A_H

#include "config.h"

void drawStoredBlocks(Grid *grid);
int clearFullRows(Grid *grid);
void playSoundEffect(const char* soundFile);
void stopSound();
int isGameOver(Grid* grid);
void drawGameOverScreen(Grid grid, int score);
void storeTetrominoInGrid(Grid *grid, Tetromino *t);

#endif