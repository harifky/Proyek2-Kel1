#include "../header/Rifky.h"
#include "../header/Micky.h"
#include "../header/config.h"

Tetromino findShadowPosition(Tetromino *t, Grid *grid) {
    Tetromino shadow = *t; // Salin tetromino saat ini

    //shadow block akan ditampilkan selama blok masih bisa jatuh
    while (canMoveDown(&shadow, grid)) {
        for (int i = 0; i < 4; i++) {
            shadow.blocks[i].y += 1;
        }
    }
    return shadow;
}

void drawShadowBlock(Tetromino *t, Grid *grid) {
    Tetromino shadow = findShadowPosition(t, grid); //Mencari posisi block yang sedang jatuh

    setcolor(WHITE);  // Set warna untuk shadowblock
    setlinestyle(SOLID_LINE, 0, 1);  // Garis putus-putus untuk efek bayangan

    //Gambar shadowblock
    for (int i = 0; i < 4; i++) {
        int x = shadow.blocks[i].x * BLOCK_SIZE + 220;
        int y = shadow.blocks[i].y * BLOCK_SIZE + 50;

        rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
    }
}