#ifndef CONFIG_H
#define CONFIG_H

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <graphics.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define screenWidth getmaxwidth()
#define screenHeight getmaxheight()
#define GRID_WIDTH 10 //Definisikan lebar Grid
#define GRID_HEIGHT 20 //Definisikan tinggi Grid
#define BLOCK_SIZE screenWidth / 40 //Definisikan ukuran blok

#define MAX_LEADERBOARD 5

extern int frameDelay;
extern int isHolding;
extern int hasHeldThisTurn;

//struktur panel tampilan skor dll
typedef struct {
    int x, y; //Inisialisasi koordinat x & y untuk panel
    int width, height; //Inisialisasi panjang dan lebar untuk panel
} Panel;

typedef struct StoredBlock {
    int x, y;
    int color;
    struct StoredBlock* next;
} StoredBlock;

//struktur tampilan grid area permainan
typedef struct {
    int x, y;
    int width, height;
    StoredBlock* blocks;  // pointer ke linked list
} Grid;

// Struktur blok individual dalam tetromino
typedef struct {
    int x, y; //inisialisasi koordinat blok
} Block;

typedef struct {
    int x, y, width, height;
} HoldBox;

extern Grid gameGrid;
extern Panel gameHoldPanel;
extern Panel gamePanel;
extern Panel leadPanel;

const Block tetrominoShapes[7][4] = {
    {{-1, 0}, {0, 0}, {1, 0}, {2, 0}},  // I (Garis)
    {{1, 0}, {0, 0}, {0, 1}, {1, 1}},   // O (Kotak)
    {{-1, 0}, {0, 0}, {1, 0}, {0, 1}},  // T
    {{-1, 0}, {0, 0}, {1, 0}, {1, 1}},  // L
    {{1, 0}, {0, 0}, {-1, 0}, {1, -1}}, // J
    {{1, 0}, {0, 0}, {0, 1}, {-1, 1}},  // Z
    {{-1, 0}, {0, 0}, {0, 1}, {1, 1}}   // S
};

// Warna untuk Tetromino
const int tetrominoColors[7] = {CYAN, YELLOW, MAGENTA, BLUE, LIGHTRED, GREEN, RED};

// Struktur Tetromino
// typedef struct {
//     int x, y;      // Posisi Tetromino di dalam grid
//     Block blocks[4]; //Ukuran dari satu blok tetromino
//     int color; // untuk warna dari tetromino
// } Tetromino;

typedef struct BlockNode {
    int x, y;
    struct BlockNode* next;
} BlockNode;

typedef struct {
    BlockNode* head;  // pointer ke blok pertama
    int color;
}Tetromino;

extern Tetromino nextTetromino;
extern Tetromino heldTetromino;
extern Tetromino currentTetromino;

#endif