#include <graphics.h>
// #include <conio.h>
// #include <time.h>
#include "../header/game.h"
#include "../header/grid.h"
#include "../header/tetromino.h"

#define TARGET_FPS 60  // Bisa diganti ke 20 jika ingin 20 FPS
#define FRAME_DELAY (1000 / TARGET_FPS)

#define DROP_SPEED 30

void playGame(){
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    Grid gameGrid = {220, 50, GRID_WIDTH * BLOCK_SIZE, GRID_HEIGHT * BLOCK_SIZE};
    Panel gameHoldPanel = {gameGrid.x - 110, 50, 100, 100};
    Panel gamePanel = {gameGrid.x + gameGrid.width + 10, 50, 110, gameGrid.height};
    
    // drawHoldPanel(gameHoldPanel);
    // drawGrid(gameGrid);
    // drawPanel(gamePanel);

    Tetromino currentTetromino = createTetromino(3, 5, 0); // Tetromino 'I' di tengah atas
    // drawTetromino(currentTetromino);

    int tick = 0;
    int currentPage = 0; // Untuk mengatur buffer aktif
    int frameCount = 0; // Counter untuk mengontrol kecepatan jatuh

    while (1) {
        if (kbhit()) {
            char key = getch();
            if (key == 75) moveTetromino(&currentTetromino, -1, 0); // Geser kiri
            if (key == 77) moveTetromino(&currentTetromino, 1, 0);  // Geser kanan
            if (key == 80) moveTetromino(&currentTetromino, 0, 1);  // Turun lebih cepat
            if (key == 72) rotateTetromino(&currentTetromino);   // putar 90°
        }

        // Tetromino hanya turun setiap DROP_SPEED frame
        if (frameCount % DROP_SPEED == 0) {
            if (canMoveDown(&currentTetromino)) {
                moveTetromino(&currentTetromino, 0, 1);
            } else {
                storeTetrominoInGrid(&gameGrid, currentTetromino); // Simpan ke grid
                currentTetromino = createTetromino(rand() % 7, 5, 0); // Spawn Tetromino baru
            }
        }
    
        // Update tampilan
        setactivepage(currentPage);  // Aktifkan halaman buffer
        cleardevice();

        drawHoldPanel(gameHoldPanel);
        drawGrid(gameGrid);
        drawPanel(gamePanel);
        drawTetromino(currentTetromino);

        setvisualpage(currentPage);  // Tampilkan halaman buffer
        currentPage = 1 - currentPage; // Ganti halaman untuk frame berikutnya


        // Update frame count
        frameCount++;

        delay(FRAME_DELAY); // Untuk animasi
    }
}