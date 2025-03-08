#include <graphics.h>
#include "../header/grid.h"
#include "../header/game.h"


void drawGrid(Grid grid) {
    for (int i = 0; i <= GRID_WIDTH; i++) {
        line(grid.x + i * BLOCK_SIZE, grid.y, grid.x + i * BLOCK_SIZE, grid.y + GRID_HEIGHT * BLOCK_SIZE);
    }
    for (int i = 0; i <= GRID_HEIGHT; i++) {
        line(grid.x, grid.y + i * BLOCK_SIZE, grid.x + GRID_WIDTH * BLOCK_SIZE, grid.y + i * BLOCK_SIZE);
    }
}

void drawHoldPanel(Panel panel){
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    char holdText[] = "Hold";

    outtextxy(panel.x + 20, panel.y + 20, holdText);
}

void drawPanel(Panel panel) {
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    
    char scoreText[] = "Score";
    char hiScoreText[] = "Hi-Score";
    char speedText[] = "Speed";
    char nextText[] = "Next";

    outtextxy(panel.x + 20, panel.y + 20, scoreText);
    outtextxy(panel.x + 20, panel.y + 50, hiScoreText);
    outtextxy(panel.x + 20, panel.y + 100, speedText);
    outtextxy(panel.x + 20, panel.y + 150, nextText);
}

void storeTetrominoInGrid(Grid *grid, Tetromino t) {
    for (int i = 0; i < 4; i++) {
        int x = t.blocks[i].x;
        int y = t.blocks[i].y;
        if (y >= 0 && y < GRID_HEIGHT && x >= 0 && x < GRID_WIDTH) {
            grid->cells[y][x] = 1; // Tandai sel sebagai terisi
        }
    }
}
