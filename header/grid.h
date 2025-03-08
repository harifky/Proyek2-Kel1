#ifndef GRID_H
#define GRID_H

#include "game.h"
#include "tetromino.h"

typedef struct {
    int x, y;
    int width, height;
} Panel;

typedef struct {
    int x, y;
    int width, height;
    int cells[GRID_HEIGHT][GRID_WIDTH]; // Grid untuk menyimpan blok
} Grid;

void drawGrid(Grid grid);

void drawPanel(Panel panel);

void drawHoldPanel(Panel panel);

void storeTetrominoInGrid(Grid *grid, Tetromino t)

#endif