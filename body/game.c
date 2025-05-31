#include <graphics.h>
#include "../header/game.h"
#include <windows.h>
#include <mmsystem.h>

// Kecepatan awal frame per detik (60 FPS)
int frameDelay = 1000 / 60;

Tetromino currentTetromino;
Tetromino nextTetromino;

// Fungsi untuk memperbarui posisi tetromino dan skor
void updateGame(Tetromino *tetromino, Grid *grid, int *score, int frameCount) {
    int dropSpeed = (frameDelay / 5 > 1) ? (frameDelay / 5) : 1;

    // Setiap interval tertentu, tetromino akan turun
    if (frameCount % dropSpeed == 0) {
        if (canMoveDown(tetromino, grid)) {
            moveTetromino(tetromino, grid, 0, 1);
        } else {
            // Simpan tetromino ke grid saat tidak bisa turun
            storeTetrominoInGrid(grid, tetromino);

            // Bersihkan baris penuh dan hitung skor
            int rowsCleared = clearFullRows(grid);
            int scoreTable[] = {0, 100, 250, 400, 800};
            int previousScore = *score;

            *score += scoreTable[rowsCleared];

            printf("Score: %d\n", *score);

            int hasHeldThisTurn = 0;

            currentTetromino = getNewTetromino();

        }
    }
}

// fungsi untuk menjalankan game
/*
Dibuat oleh
- Hafizh Andika 
- M. Naufal Alfarizky
- M. Naufal Nurmaryadi
- Micky Ridho
- Rifky Hermawan 
*/
void playGame(){

    int gd = DETECT, gm;
    initwindow(screenWidth, screenHeight, "Tetris Fullscreen", -3, -3);

    // Inisialisasi posisi dan ukuran grid serta panel
    Grid gameGrid = {400, 50, GRID_WIDTH * BLOCK_SIZE, GRID_HEIGHT * BLOCK_SIZE};
    Panel gameHoldPanel = {gameGrid.x - 160, gameGrid.y + (gameGrid.height - 750), 150, 150};
    Panel gamePanel = {gameGrid.x + gameGrid.width + 10, 50, 200, gameGrid.height};
    Panel leadPanel = {gameGrid.x + gameGrid.width + gamePanel.width + 20, 50, 400, 300};
    
    bool isRunning = true; //trigger untuk memulai game
    int currentPage = 0; // Untuk mengatur buffer aktif
    int frameCount = 0; // Counter untuk mengontrol kecepatan jatuh
    int score = 0; //inisialisasi awal score

    srand(time(NULL));

    StoredBlock *current = gameGrid.blocks;
    while (current != NULL) {
        StoredBlock *temp = current;
        current = current->next;
        free(temp);
    }
    gameGrid.blocks = NULL;
    

    //Inisialisasi currentTetromino & nextTetromino**
    nextTetromino = createTetromino(setRandomTetromino(), 5, -2);
    currentTetromino = createTetromino(setRandomTetromino(), 5, -3);

    
    

    //memutar backsound
    playSoundEffect("sound/HoldOnTight.wav");
 
    while (isRunning) {

        handleInput(&currentTetromino, &gameGrid, &score);

        // Perbarui posisi tetromino, skor, dll.
        updateGame(&currentTetromino, &gameGrid, &score, frameCount);
        
        // Aktifkan halaman buffer
        setactivepage(currentPage);
        cleardevice();  // Bersihkan layar

        // Periksa kondisi game over
        if (isGameOver(&gameGrid)) {
            setvisualpage(currentPage);  // Tampilkan buffer saat ini

            // Tampilkan UI akhir game
            drawPanel(gamePanel, &score);
            drawGrid(gameGrid);
            drawLeadPanel(leadPanel);
            drawGameOverScreen(gameGrid, score);

            stopSound();     // Hentikan musik
            isRunning = false;
            break;
        }else {
            drawGrid(gameGrid);
        }

        drawHoldPanel(gameHoldPanel);
        
        drawPanel(gamePanel, &score);

        drawLeadPanel(leadPanel);

        drawShadowBlock(&currentTetromino, &gameGrid);
        
        drawStoredBlocks(&gameGrid); 

        drawTetromino(currentTetromino);

        setvisualpage(currentPage);  // Tampilkan halaman buffer
        currentPage = 1 - currentPage; // Ganti halaman untuk frame berikutnya


        // Update frame count untuk control kecepatan tetromino
        frameCount++;

        // Update frame delay berdasarkan skor
        updateFrameDelay(&score); 
        delay(frameDelay);

    }
}