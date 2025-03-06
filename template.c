#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

#define WIDTH  520
#define HEIGHT 640
#define SCREEN_WIDTH  280
#define SCREEN_HEIGHT 500
#define BLOCK_SIZE    20

#define GRID_X 100     // Posisi awal grid di layar
#define GRID_Y 0


typedef enum { I, O, T, L, J, S, Z } TetrominoType;

// Koordinat relatif untuk setiap tetromino
int offsets[7][4][2] = {
    { {0,1}, {1,1}, {2,1}, {3,1} },  // I
    { {0,0}, {1,0}, {0,1}, {1,1} },  // O
    { {1,0}, {0,1}, {1,1}, {2,1} },  // T
    { {2,0}, {0,1}, {1,1}, {2,1} },  // L
    { {0,0}, {0,1}, {1,1}, {2,1} },  // J
    { {1,0}, {2,0}, {0,1}, {1,1} },  // S
    { {0,0}, {1,0}, {1,1}, {2,1} }   // Z
};

// Warna untuk setiap tetromino
int tetrominoColors[7] = { CYAN, YELLOW, MAGENTA, WHITE, BLUE, GREEN, RED };

// Fungsi untuk menggambar tetromino
void drawTetromino(int x, int y, TetrominoType type) {
    setfillstyle(SOLID_FILL, tetrominoColors[type]);
    
    for (int i = 0; i < 4; i++) {
        int blockX = GRID_X + (x + offsets[type][i][0]) * BLOCK_SIZE;
        int blockY = GRID_Y + (y + offsets[type][i][1]) * BLOCK_SIZE;
        bar(blockX, blockY, blockX + BLOCK_SIZE, blockY + BLOCK_SIZE);
    }
}

// Fungsi delay sederhana menggunakan clock
void delayMillis(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds);
}


// Struktur untuk menyimpan data permainan
typedef struct {
    int score;
    int level;
    int fallSpeed = 500;
    int tetrominoX = 4;  // Posisi grid (bukan piksel)
    int tetrominoY = 0;
    int board[20][10];
    int currentBlock[4][4];
    int nextBlock[4][4];
    int holdBlock[4][4];
    int ghostBlock[4][4];
} GameState;

GameState game;
TetrominoType currentTetronimo;

void initGraphics() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
}

// Menggambar papan permainan
void drawBoard() {
    // TODO: Implementasi menggambar papan dan blok
    for (int x = 0; x <= WIDTH; x += BLOCK_SIZE) {
        for (int y = 0; y <= HEIGHT; y += BLOCK_SIZE) {
            // rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
        }
    }
}

void drawScreen(){
    for (int x = 100; x <= SCREEN_WIDTH; x += BLOCK_SIZE) {
        for (int y = 0; y <= SCREEN_HEIGHT; y += BLOCK_SIZE) {
            rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
            setbkcolor(GREEN);
        }
    }
}

// Menggambar skor dan level
void drawScore() {
    // TODO: Implementasi menggambar skor dan level
}

// Menggambar blok saat ini
void drawCurrentBlock() {
    // TODO: Implementasi menggambar blok yang sedang dimainkan
}

// Menampilkan blok berikutnya
void drawNextBlock() {
    // TODO: Implementasi menggambar blok berikutnya
}

// Menampilkan blok yang di-hold
void drawHoldBlock() {
    // TODO: Implementasi menggambar blok yang di-hold
}

// Menampilkan ghost block
void drawGhostBlock() {
    // TODO: Implementasi menggambar ghost block
}

// Menampilkan layar game over
void gameOverScreen() {
    // TODO: Implementasi layar game over
}

// Menyimpan permainan
void saveGame() {
    // TODO: Implementasi menyimpan permainan ke file
}

// Memuat permainan
void loadGame() {
    // TODO: Implementasi memuat permainan dari file
}

// Fungsi utama permainan
void gameLoop() {
    while (1) {
        cleardevice();
        drawBoard();
        drawScore();
        drawCurrentBlock();
        drawNextBlock();
        drawHoldBlock();
        drawGhostBlock();
        delay(100);
        
        if (kbhit()) {
            char key = getch();
            if (key == 27) break; // ESC untuk keluar
            // TODO: Tambahkan kontrol permainan
        }
    }
}

// void screenRefresh(GameState game, TetrominoType currentTetronimo){

//     while(true){

//         cleardevice();
        
//         drawTetromino(game.tetrominoX, game.tetrominoY, currentTetronimo);

//         game.tetrominoY++;
//         delayMillis(game.fallSpeed);

        
//     }
// }

int main() {
    initGraphics();
    // drawBoard();
    drawScreen();
    // drawTetromino(140, 80, BLUE);
    // gameLoop();

    // drawTetromino(GRID_X, GRID_Y, T);
    currentTetronimo = J;

    while(1){

        cleardevice();
        
        drawTetromino(game.tetrominoX, game.tetrominoY, currentTetronimo);

        game.tetrominoY++;
        delayMillis(game.fallSpeed);

        
    }

    getch();
    closegraph();
    return 0;
}
