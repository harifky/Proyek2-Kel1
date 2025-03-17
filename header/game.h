#ifndef GAME_H
#define GAME_H

#include "grid.h" 
#include "tetromino.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define screenWidth getmaxwidth()
#define screenHeight getmaxheight()
#define GRID_WIDTH 10 //Definisikan lebar Grid
#define GRID_HEIGHT 20 //Definisikan tinggi Grid
#define BLOCK_SIZE screenWidth / 40 //Definisikan ukuran blok

void handleInput(Tetromino *tetromino, Grid *grid, int *score); // fungsi untuk menangani input user

void updateGame(Tetromino *tetromino, Grid *grid, int *score, int frameCount); //fungsi untuk memperbarui tampilan layar

void playGame(); // fungsi untuk menjalankan game

#endif