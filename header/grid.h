#ifndef GRID_H
#define GRID_H

#include "tetromino.h"

#define GRID_WIDTH 10 //Definisikan lebar Grid
#define GRID_HEIGHT 20 //Definisikan panjang Grid

//struktur tampilan grid area permainan
typedef struct {
    int x, y; //Inisialisasi koordinat x & y untuk grid
    int width, height;  //Inisialisasi panjang dan lebar untuk grid
    int cells[GRID_HEIGHT][GRID_WIDTH]; // Grid untuk menyimpan blok
} Grid;

//struktur panel tampilan skor dll
typedef struct {
    int x, y; //Inisialisasi koordinat x & y untuk panel
    int width, height; //Inisialisasi panjang dan lebar untuk panel
} Panel;

void drawGrid(Grid grid); //fungsi untuk menggambar Grid

void drawPanel(Panel panel, int *score, Tetromino *nextTetromino); //fungsi untuk menggambar panel

void drawNextPreviewBlock(Panel panel, Tetromino *nextTetromino);

void drawHoldPanel(Panel panel); //fungsi untuk menggambar holdPanel

void drawStoredBlocks(Grid *grid); //fungsi untuk menggambar blok yang sudah disimpan dalam grid

int clearFullRows(Grid *grid); //fungsi untuk menghapus baris yang sudah penuh

void drawScore(Panel panel, int score);

#endif