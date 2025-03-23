#ifndef GAME_H
#define GAME_H

#include "config.h" 
#include "grid.h" 
#include "tetromino.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void handleInput(Tetromino *tetromino, Grid *grid, int *score); // fungsi untuk menangani input user

void updateGame(Tetromino *tetromino, Grid *grid, int *score, int frameCount); //fungsi untuk memperbarui tampilan layar

void playGame(); // fungsi untuk menjalankan game

void holdTetromino(Tetromino *current);// fungsi untuk fitur holg block

void updateFrameDelay(int *score); // fungsi untuk mengatur speed jika sudah sampai ke skor tertentu

#endif