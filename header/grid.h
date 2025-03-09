#ifndef GRID_H
#define GRID_H

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

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

void drawPanel(Panel panel, int score);

void drawHoldPanel(Panel panel);

void drawStoredBlocks(Grid *grid);

int clearFullRows(Grid *grid);

#endif