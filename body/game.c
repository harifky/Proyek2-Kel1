#include <graphics.h>
#include "../header/game.h"
#include <windows.h>
#include <mmsystem.h>

//#define TARGET_FPS 60  // Bisa diganti ke 20 jika ingin 20 FPS
//#define FRAME_DELAY (1000 / TARGET_FPS)
//#define DROP_SPEED 20

int frameDelay = 1000 / 60;
Tetromino currentTetromino;
Tetromino nextTetromino;
// Grid grid;

//fungsi untuk memperbarui tampilan layar
/*Dibuat oleh M. Naufal Nurmaryadi & M. Naufal Alfarizky*/
void updateGame(Tetromino *tetromino, Grid *grid, int *score, int frameCount) {
    int dropSpeed = (frameDelay / 5 > 1) ? (frameDelay / 5) : 1;
    if (frameCount % dropSpeed == 0) {  
        if (canMoveDown(tetromino, grid)) {
            moveTetromino(tetromino, grid, 0, 1);
        } else {
            storeTetrominoInGrid(grid, tetromino);
            int rowsCleared = clearFullRows(grid);

            int scoreTable[] = {0, 200, 300, 500, 800};
            *score += scoreTable[rowsCleared];

            printf("Score: %d\n", *score);

            *tetromino = nextTetromino;

            // *tetromino = createTetromino(setRandomTetromino(), 5, -2);
            int hasHeldThisTurn = 0;

            // currentTetromino = getNewTetromino();

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
    
    // Buat window fullscreen
    initwindow(screenWidth, screenHeight, "Tetris Fullscreen", -3, -3);
    
    //Inisialisasi ukuran grid dan panel untuk tampilan game
    Grid gameGrid = {400, 50, GRID_WIDTH * BLOCK_SIZE, GRID_HEIGHT * BLOCK_SIZE};
    // Panel Hold di tengah sisi kiri grid utama
    Panel gameHoldPanel = {gameGrid.x - 160, gameGrid.y + (gameGrid.height - 650), 150, 150};

    Panel gamePanel = {gameGrid.x + gameGrid.width + 10, 50, 200, gameGrid.height};
    Panel leadPanel = {gameGrid.x + gameGrid.width + gamePanel.width + 20, 50, 400, 300};
    
    bool start = true; //trigger untuk memulai game
    int currentPage = 0; // Untuk mengatur buffer aktif
    int frameCount = 0; // Counter untuk mengontrol kecepatan jatuh
    int score = 0; //inisialisasi awal score

    // // Inisialisasi grid dengan mengisi sel dengan nilai 0 (kosong)
    // for (int y = 0; y < GRID_HEIGHT; y++) {
    //     for (int x = 0; x < GRID_WIDTH; x++) {
    //         gameGrid.cells[y][x] = 0;  // Pastikan grid kosong saat memulai permainan
    //     }
    // }


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
 
    while (start) {

        // printf("Start Loop!!\n");

        handleInput(&currentTetromino, &gameGrid, &score);

        updateGame(&currentTetromino, &gameGrid, &score, frameCount);
    
        // Aktifkan halaman buffer
        setactivepage(currentPage);
        cleardevice();
        
        // printf("draw hold Panel!!\n");
        drawHoldPanel(gameHoldPanel);

        //Cek apakah game over
        if (isGameOver(&gameGrid)) {
            
            setvisualpage(currentPage);
            
            drawPanel(gamePanel, &score);
            
            drawGrid(gameGrid);
            
            drawLeadPanel(leadPanel);
            
            drawGameOverScreen(gameGrid, score);
            
            stopSound();

            start = false;
            
            break;
        }else {
            drawGrid(gameGrid);
        }
        
        // printf("draw  Panel!!\n");
        drawPanel(gamePanel, &score);

        // printf("draw lead Panel!!\n");
        drawLeadPanel(leadPanel);

        // printf("draw Shadowblock!!\n");
        drawShadowBlock(&currentTetromino, &gameGrid);
        
        // printf("draw Stored Block!!\n");
        drawStoredBlocks(&gameGrid); 

        // printf("draw tetromino!!\n");
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