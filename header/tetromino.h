#ifndef TETROMINO_H
#define TETROMINO_H

// Struktur blok individual dalam tetromino
typedef struct {
    int x, y;
} Block;

// Struktur Tetromino
typedef struct {
    Block blocks[4];
    int color;
} Tetromino;

// Fungsi untuk membuat Tetromino
Tetromino createTetromino(int type, int startX, int startY);

// Fungsi untuk menggambar Tetromino
void drawTetromino(Tetromino t);

void moveTetromino(Tetromino *t, int dx, int dy);

void rotateTetromino(Tetromino *t);

int canMoveDown(Tetromino *t);

#endif