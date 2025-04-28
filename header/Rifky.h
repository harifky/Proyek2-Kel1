#ifndef RIFKY_H
#define RIFKY_H

#include "config.h" 

Tetromino findShadowPosition(Tetromino *t, Grid *grid);
void drawShadowBlock(Tetromino *t, Grid *grid);
void drawLeadPanel(Panel panel);
void saveScoreToFile(const char *username, int score);

#endif