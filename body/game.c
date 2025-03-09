#include <graphics.h>
#include "../header/game.h"
#include "../header/grid.h"
#include "../header/tetromino.h"

#define TARGET_FPS 60  // Bisa diganti ke 20 jika ingin 20 FPS
#define FRAME_DELAY (1000 / TARGET_FPS)
#define DROP_SPEED 20

// #define score 0 

void handleInput(Tetromino *tetromino, Grid *grid, int *score) {
    if (kbhit()) {
        char key = getch();
        if (key == 75) moveTetromino(tetromino, grid, -1, 0);
        if (key == 77) moveTetromino(tetromino, grid, 1, 0);
        if (key == 80) moveTetromino(tetromino, grid, 0, 1);
        if (key == 32) hardDropTetromino(tetromino, grid, score);
        if (key == 72) rotateTetromino(tetromino, grid);
    }
}

void updateGame(Tetromino *tetromino, Grid *grid, int *score, int frameCount) {
    // if (frameCount % DROP_SPEED == 0) {
    //     if (canMoveDown(tetromino, grid)) {
    //         moveTetromino(tetromino, grid, 0, 1);
    //     } else {
    //         storeTetrominoInGrid(grid, tetromino);
    //         int rowsCleared = clearFullRows(grid);

    //         // Skor berdasarkan jumlah baris yang dihapus
    //         int scoreTable[] = {0, 100, 300, 500, 800};
    //         *score += scoreTable[rowsCleared];
    //         printf("skornya : %d\n", *score);

    //         *tetromino = createTetromino(setRandomTetromino(), 5, -2);
    //     }
    // }

    if (frameCount % DROP_SPEED == 0) {
        if (canMoveDown(tetromino, grid)) {
            moveTetromino(tetromino, grid, 0, 1);
        } else {
            storeTetrominoInGrid(grid, tetromino); // Simpan blok di grid

            printf("Memeriksa baris penuh...\n");
            int rowsCleared = clearFullRows(grid);
            printf("Baris dihapus: %d\n", rowsCleared); // DEBUGGING
            printf("Baris dihapus:ufqeyjhkajslnd;ckoesuk \n"); // DEBUGGING

            // Tambahkan skor berdasarkan jumlah baris yang dihapus
            if (rowsCleared == 1) {
                *score += 100;
            } else if (rowsCleared == 2) { 
                *score += 300;
            } else if (rowsCleared == 3) {
                *score += 500;
            } else if (rowsCleared == 4) {
                *score += 800;
            }

            printf("Score setelah update: %d\n", *score); // Debugging

            // Buat tetromino baru
            *tetromino = createTetromino(setRandomTetromino(), 5, -2);
        }
    }
}

void playGame(){
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
    
    int tick = 0;
    int currentPage = 0; // Untuk mengatur buffer aktif
    int frameCount = 0; // Counter untuk mengontrol kecepatan jatuh
    int score = 0;
    
    Grid gameGrid = {220, 50, GRID_WIDTH * BLOCK_SIZE, GRID_HEIGHT * BLOCK_SIZE};
    Panel gameHoldPanel = {gameGrid.x - 110, 50, 100, 100};
    Panel gamePanel = {gameGrid.x + gameGrid.width + 10, 50, 110, gameGrid.height};

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            gameGrid.cells[y][x] = 0;  // Pastikan grid kosong saat memulai permainan
        }
    }
    
    Tetromino currentTetromino;
    currentTetromino = createTetromino(setRandomTetromino(), 5, -3); // Tetromino 'I' di tengah atas


    while (1) {

        handleInput(&currentTetromino, &gameGrid, &score);

        updateGame(&currentTetromino, &gameGrid, &score, frameCount);
    
        // Update tampilan
        setactivepage(currentPage);  // Aktifkan halaman buffer
        cleardevice();

        drawHoldPanel(gameHoldPanel);
        
        drawGrid(gameGrid);
        
        drawPanel(gamePanel, &score);
        
        drawStoredBlocks(&gameGrid);  // Gambar blok yang sudah tersimpan

        drawTetromino(currentTetromino);

        setvisualpage(currentPage);  // Tampilkan halaman buffer
        currentPage = 1 - currentPage; // Ganti halaman untuk frame berikutnya


        // Update frame count
        frameCount++;

        delay(FRAME_DELAY); // Untuk animasi
    }
}