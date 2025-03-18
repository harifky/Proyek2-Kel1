#ifndef GRID_H
#define GRID_H

#include "config.h"

//struktur panel tampilan skor dll
typedef struct {
    int x, y; //Inisialisasi koordinat x & y untuk panel
    int width, height; //Inisialisasi panjang dan lebar untuk panel
} Panel;

//struktur tampilan grid area permainan
typedef struct {
    int x, y; //Inisialisasi koordinat x & y untuk grid
    int width, height;  //Inisialisasi panjang dan lebar untuk grid
    int cells[GRID_HEIGHT][GRID_WIDTH]; // Grid untuk menyimpan blok
} Grid;

void drawGrid(Grid grid); //fungsi untuk menggambar Grid

void drawPanel(Panel panel, int *score); //fungsi untuk menggambar panel

void drawHoldPanel(Panel panel); //fungsi untuk menggambar holdPanel

void drawStoredBlocks(Grid *grid); //fungsi untuk menggambar blok yang sudah disimpan dalam grid

int clearFullRows(Grid *grid); //fungsi untuk menghapus baris yang sudah penuh

void drawScore(Panel panel, int score);

void playSoundEffect(const char* soundFile);

void stopSound();

int isGameOver(Grid* grid);

void drawGameOverScreen();

#endif