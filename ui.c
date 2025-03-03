#include <graphics.h>
#include "ui.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define BLOCK_SIZE    20

void drawUI() {
    setcolor(WHITE);

    // Kotak Skor
    rectangle(20, 20, 120, 50);
    outtextxy(40, 30, "SCORE");

    // Kotak Timer
    rectangle(20, 60, 120, 90);
    outtextxy(40, 70, "TIMER");

    // Kotak Next Block (dengan warna ungu)
    setcolor(MAGENTA);
    rectangle(140, 20, 240, 90);
    outtextxy(160, 50, "NEXT BLOCK");

    // Kotak Game Board
    setcolor(WHITE);
    rectangle(50, 110, 350, 450);
    outtextxy(170, 280, "GAME BOARD");
}
