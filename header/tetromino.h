#ifndef TETROMINO_H
#define TETROMINO_H

#include "grid.h"

// Struktur blok individual dalam tetromino
typedef struct {
    int x, y; //inisialisasi koordinat blok
} Block;

// Struktur Tetromino
typedef struct {
    int x, y;      // Posisi Tetromino di dalam grid
    Block blocks[4]; //Ukuran dari satu blok tetromino
    int color; // untuk warna dari tetromino
} Tetromino;

// Fungsi untuk membuat Tetromino
Tetromino createTetromino(int type, int startX, int startY);

// Fungsi untuk menggambar Tetrominoz
void drawTetromino(Tetromino t);

// fungsi untuk menggerakkan blok tetromino
void moveTetromino(Tetromino *t, Grid *grid, int dx, int dy);

//fungsi untuk memutar blok tetromino
void rotateTetromino(Tetromino *t, Grid *grid);

//fungsi untuk mengecek apakah tetromino dapat jatuh atau tidak
int canMoveDown(Tetromino *t, Grid *grid);

//fungsi untuk menjatuhkan tetromino langsung ke bawah
void hardDropTetromino(Tetromino *t, Grid *grid, int *score);

//fungsi untuk mengecek apakah tetromino didalam area yang dapat bergerak atau digerakkan
int canMoveTetromino(Tetromino *t, Grid *grid, int dx, int dy);

//fungsi untuk menyimpan tetromino didalam grid
void storeTetrominoInGrid(Grid *grid, Tetromino *t);

//fungsi untuk menentukan tetromino secara acak
int setRandomTetromino();

Tetromino findShadowPosition(Tetromino *t, Grid *grid);

void drawShadowBlock(Tetromino *t, Grid *grid);

#endif