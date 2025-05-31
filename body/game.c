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

            // Putar suara jika skor bertambah
            if (*score > previousScore) {
                playSoundEffect("sound/Point.wav");
            }

            printf("Score: %d\n", *score);

            // Tetromino berikutnya menjadi aktif, dan buat yang baru untuk "next"
            *tetromino = nextTetromino;
            nextTetromino = createTetromino(setRandomTetromino(), 5, -2);
        }
    }
}

// Fungsi utama untuk menjalankan game
void playGame() {
    int gd = DETECT, gm;
    initwindow(screenWidth, screenHeight, "Tetris Fullscreen", -3, -3);

    // Inisialisasi posisi dan ukuran grid serta panel
    Grid gameGrid = {400, 50, GRID_WIDTH * BLOCK_SIZE, GRID_HEIGHT * BLOCK_SIZE};
    Panel holdPanel = {gameGrid.x - 160, gameGrid.y + (gameGrid.height - 650), 150, 150};
    Panel infoPanel = {gameGrid.x + gameGrid.width + 10, 50, 200, gameGrid.height};
    Panel leaderboardPanel = {infoPanel.x + infoPanel.width + 10, 50, 400, 300};

    bool isRunning = true;
    int currentPage = 0;  // Untuk double buffering
    int frameCount = 0;   // Hitungan frame berjalan
    int score = 0;;

    // Buat tetromino awal
    nextTetromino = createTetromino(setRandomTetromino(), 5, -2);
    currentTetromino = createTetromino(setRandomTetromino(), 5, -3);

    // Mainkan musik latar
    playSoundEffect("sound/HoldOnTight.wav");

    // Game loop utama
    while (isRunning) {
        // Tangani input user (panah, rotasi, drop, dll)
        handleInput(&currentTetromino, &gameGrid, &score);

        // Perbarui posisi tetromino, skor, dll.
        updateGame(&currentTetromino, &gameGrid, &score, frameCount);

        // Aktifkan halaman buffer untuk menggambar
        setactivepage(currentPage);
        cleardevice();  // Bersihkan layar

        // Periksa kondisi game over
        if (isGameOver(&gameGrid)) {
            setvisualpage(currentPage);  // Tampilkan buffer saat ini

            // Tampilkan UI akhir game
            drawPanel(infoPanel, &score);
            drawGrid(gameGrid);
            drawLeadPanel(leaderboardPanel);
            drawGameOverScreen(gameGrid, score);

            stopSound();     // Hentikan musik
            isRunning = false;
            break;
        }

        // Gambar semua elemen UI game
        drawGrid(gameGrid);
        drawPanel(infoPanel, &score);
        drawLeadPanel(leaderboardPanel);
        drawHoldPanel(holdPanel);
        drawShadowBlock(&currentTetromino, &gameGrid);  // Bayangan posisi jatuh
        drawStoredBlocks(&gameGrid);                    // Blok yang sudah terkunci
        drawTetromino(currentTetromino);                // Tetromino yang sedang aktif

        // Tampilkan frame ke layar
        setvisualpage(currentPage);
        currentPage = 1 - currentPage;  // Ganti buffer

        frameCount++;                   // Tambahkan hitungan frame
        updateFrameDelay(&score);      // Sesuaikan kecepatan berdasarkan skor
        delay(frameDelay);             // Tunggu waktu frame
    }
}