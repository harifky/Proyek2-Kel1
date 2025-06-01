#ifndef RIFKY_H
#define RIFKY_H

#include "config.h" 

typedef struct Scoring {
    char username[50];
    int score;
    char time[30];
    struct Scoring *next;
} Scoring;

Tetromino findShadowPosition(Tetromino *t, Grid *grid);
void drawShadowBlock(Tetromino *t, Grid *grid);
void drawLeadPanel(Panel panel);
void saveScoreToFile(const char *username, int score);

#endif