#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

const int SCREEN_WIDTH = 200;
const int SCREEN_HEIGHT = 400;
const int BLOCK_SIZE = 20;
const int GRID_WIDTH = SCREEN_WIDTH / BLOCK_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / BLOCK_SIZE;

// Struktur bentuk blok
struct BlockShape {
    int coords[4][2]; // Koordinat relatif blok
    int color; // Warna blok
};

BlockShape shapes[] = {
    {{{0, 0}, {0, 1}, {0, 2}, {0, 3}}, CYAN},      // Straight
    {{{0, 0}, {1, 0}, {1, 1}, {1, 2}}, BLUE},      // L-shape
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, YELLOW},    // Square
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, GREEN},     // Z-shape
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}}, MAGENTA}    // T-shape
};

// Fungsi menggambar satu blok
void drawBlock(int x, int y, int color) {
    setfillstyle(SOLID_FILL, color);
    bar(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
    rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
}

// Fungsi menggambar latar belakang
void drawBackground() {
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    setcolor(WHITE);
    for (int i = 0; i < SCREEN_WIDTH; i += BLOCK_SIZE)
        line(i, 0, i, SCREEN_HEIGHT);
    for (int i = 0; i < SCREEN_HEIGHT; i += BLOCK_SIZE)
        line(0, i, SCREEN_WIDTH, i);
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
    srand(time(0));
    
    drawBackground(); // Menggambar latar belakang
    
    // Menampilkan contoh blok di tengah layar
    BlockShape exampleShape = shapes[0];
    drawBlock(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, exampleShape.color);
    
    getch();
    closegraph();
    return 0;
}
