#ifndef GAME_H
#define GAME_H

#include "config.h" 
#include "Rifky.h"
#include "Naufal_N.h"
#include "Hafizh.h"
#include "Micky.h"
#include "Naufal_A.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void updateGame(Tetromino *tetromino, Grid *grid, int *score, int frameCount); //fungsi untuk memperbarui tampilan layar

void playGame(); // fungsi untuk menjalankan game



#endif