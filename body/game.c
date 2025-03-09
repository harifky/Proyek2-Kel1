#include <graphics.h>
#include "../header/game.h"
#include "../header/grid.h"
#include "../header/tetromino.h"

#define TARGET_FPS 60  // Bisa diganti ke 20 jika ingin 20 FPS
#define FRAME_DELAY (1000 / TARGET_FPS)

#define DROP_SPEED 20

int tick = 0;
int currentPage = 0; // Untuk mengatur buffer aktif
int frameCount = 0; // Counter untuk mengontrol kecepatan jatuh
int score = 0;

void playGame(){
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    
    Grid gameGrid = {220, 50, GRID_WIDTH * BLOCK_SIZE, GRID_HEIGHT * BLOCK_SIZE};
    Panel gameHoldPanel = {gameGrid.x - 110, 50, 100, 100};
    Panel gamePanel = {gameGrid.x + gameGrid.width + 10, 50, 110, gameGrid.height};

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            gameGrid.cells[y][x] = 0;  // Pastikan grid kosong saat memulai permainan
        }
    }
    
    Tetromino currentTetromino;
    // = createTetromino(setRandomTetromino(), 5, -3); // Tetromino 'I' di tengah atas


    while (1) {
        if (kbhit()) {
            char key = getch();
            if (key == 75) moveTetromino(&currentTetromino, &gameGrid,  -1, 0); // Geser kiri
            if (key == 77) moveTetromino(&currentTetromino, &gameGrid,  1, 0);  // Geser kanan
            if (key == 80) moveTetromino(&currentTetromino, &gameGrid,  0, 1);  // Turun lebih cepat
            if (key == 32) hardDropTetromino(&currentTetromino, &gameGrid);  // Turun lebih cepat
            if (key == 72) rotateTetromino(&currentTetromino, &gameGrid);   // putar 90°
        }

        // Tetromino hanya turun setiap DROP_SPEED frame
        if (frameCount % DROP_SPEED == 0) {
            if (canMoveDown(&currentTetromino, &gameGrid)) {
                moveTetromino(&currentTetromino, &gameGrid, 0, 1);
            } else {
                storeTetrominoInGrid(&gameGrid, &currentTetromino); // Simpan blok di grid

                int rowsCleared = clearFullRows(&gameGrid);

                // Tambahkan skor berdasarkan jumlah baris yang dihapus
                if (rowsCleared == 1) {
                    score += 100;
                }else if (rowsCleared == 2) { 
                    score += 300;
                }else if (rowsCleared == 3) {
                    score += 500;
                }else if (rowsCleared == 4) {
                    score += 800;
                }

                clearFullRows(&gameGrid);
                currentTetromino = createTetromino(setRandomTetromino(), 5, -2); // Spawn tetromino baru
            }
        }
    
        // Update tampilan
        setactivepage(currentPage);  // Aktifkan halaman buffer
        cleardevice();

        drawHoldPanel(gameHoldPanel);
        
        drawGrid(gameGrid);
        
        drawPanel(gamePanel, score);
        
        drawStoredBlocks(&gameGrid);  // Gambar blok yang sudah tersimpan

        drawTetromino(currentTetromino);

        setvisualpage(currentPage);  // Tampilkan halaman buffer
        currentPage = 1 - currentPage; // Ganti halaman untuk frame berikutnya


        // Update frame count
        frameCount++;

        delay(FRAME_DELAY); // Untuk animasi
    }
}