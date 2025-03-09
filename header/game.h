#ifndef GAME_H
#define GAME_H

#include "grid.h" 
#include "tetromino.h"

#define GRID_WIDTH 10 //Definisikan lebar Grid
#define GRID_HEIGHT 20 //Definisikan tinggi Grid
#define BLOCK_SIZE 20 //Definisikan ukuran blok

void handleInput(Tetromino *tetromino, Grid *grid, int *score); // fungsi untuk menangani input user

void updateGame(Tetromino *tetromino, Grid *grid, int *score, int frameCount); //fungsi untuk memperbarui tampilan layar

void playGame(); // fungsi untuk menjalankan game

#endif