#include <graphics.h>
#include "../header/game.h"
#include "../header/grid.h"
#include "../header/tetromino.h"

//#define TARGET_FPS 60  // Bisa diganti ke 20 jika ingin 20 FPS
//#define FRAME_DELAY (1000 / TARGET_FPS)
//#define DROP_SPEED 20

int frameDelay = 1000 / 60;
Tetromino currentTetromino;

void handleInput(Tetromino *tetromino, Grid *grid, int *score) {
    if (kbhit()) {
        char key = getch();
        if (key == 75) moveTetromino(tetromino, grid, -1, 0);
        if (key == 77) moveTetromino(tetromino, grid, 1, 0);
        if (key == 80) moveTetromino(tetromino, grid, 0, 1);
        if (key == 32) hardDropTetromino(tetromino, grid, score);
        if (key == 72) rotateTetromino(tetromino, grid);
        if (key == 13) holdTetromino(tetromino);
    }
}

void updateGame(Tetromino *tetromino, Grid *grid, int *score, int frameCount) {
    int dropSpeed = (frameDelay / 5 > 1) ? (frameDelay / 5) : 1;
    if (frameCount % dropSpeed == 0) {  
        if (canMoveDown(tetromino, grid)) {
            moveTetromino(tetromino, grid, 0, 1);
        } else {
            storeTetrominoInGrid(grid, tetromino);
            int rowsCleared = clearFullRows(grid);

            int scoreTable[] = {0, 100, 300, 500, 800};
            *score += scoreTable[rowsCleared];

            printf("Score: %d\n", *score);

            *tetromino = nextTetromino;

            *tetromino = createTetromino(setRandomTetromino(), 5, -2);
            int hasHeldThisTurn = 0;

            currentTetromino = getNewTetromino();

        }
    }
}

void updateFrameDelay(int *score) {
    if (*score >= 4000) {
        frameDelay = 150;  // Sangat cepat
    } else if (*score >= 3000) {
        frameDelay = 300;
    } else if (*score >= 2000) {
        frameDelay = 600;
    } else if (*score >= 1000) {
        frameDelay = 1200;
    } else {
        frameDelay = 2000;  // Kecepatan awal (1 detik per drop)
    }
}


void playGame(){
    int gd = DETECT, gm;
    
    // Buat window fullscreen
    initwindow(screenWidth, screenHeight, "Tetris Fullscreen", -3, -3); // -3 agar benar-benar fullscreen
    
    Grid gameGrid = {220, 50, GRID_WIDTH * BLOCK_SIZE, GRID_HEIGHT * BLOCK_SIZE};
    Panel gameHoldPanel = {gameGrid.x - 110, 100, 100, 100};
    Panel gamePanel = {gameGrid.x + gameGrid.width + 10, 50, 110, gameGrid.height};
    Panel leadPanel = {gameGrid.x + gameGrid.width + gamePanel.width + 20, 50, 250, 300};
    
    bool start = true;
    int tick = 0;
    int currentPage = 0; // Untuk mengatur buffer aktif
    int frameCount = 0; // Counter untuk mengontrol kecepatan jatuh
    int score = 0;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            gameGrid.cells[y][x] = 0;  // Pastikan grid kosong saat memulai permainan
        }
    }
    
    // **Inisialisasi currentTetromino & nextTetromino**
    nextTetromino = createTetromino(setRandomTetromino(), 5, -2);
    currentTetromino = createTetromino(rand() % 7, 5, -3);
    playSoundEffect("sound/HoldOnTight.wav");
    
    while (start) {

        handleInput(&currentTetromino, &gameGrid, &score);

        updateGame(&currentTetromino, &gameGrid, &score, frameCount);
    
        // Update tampilan
        setactivepage(currentPage);  // Aktifkan halaman buffer
        cleardevice();

        drawHoldPanel(gameHoldPanel);

        if (isGameOver(&gameGrid)) {
            setvisualpage(currentPage);
            drawPanel(gamePanel, &score);
            stopSound();
            drawGrid(gameGrid);
            drawLeadPanel(leadPanel);
            drawGameOverScreen(gameGrid, score);
            start = false;
            break;
        }else {
            drawGrid(gameGrid);
        }
        
        drawPanel(gamePanel, &score);

        drawLeadPanel(leadPanel);

        drawShadowBlock(&currentTetromino, &gameGrid);
        
        drawStoredBlocks(&gameGrid);  // Gambar blok yang sudah tersimpan

        drawTetromino(currentTetromino);

        setvisualpage(currentPage);  // Tampilkan halaman buffer
        currentPage = 1 - currentPage; // Ganti halaman untuk frame berikutnya


        // Update frame count
        frameCount++;

        updateFrameDelay(&score); // Update frame delay berdasarkan skor
    }
}