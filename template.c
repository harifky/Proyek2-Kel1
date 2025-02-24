#include <graphics.h>
#include <conio.h>
#include <stdio.h>

#define SCREEN_WIDTH  620
#define SCREEN_HEIGHT 440
#define BLOCK_SIZE    20

// Struktur untuk menyimpan data permainan
typedef struct {
    int score;
    int level;
    int board[20][10];
    int currentBlock[4][4];
    int nextBlock[4][4];
    int holdBlock[4][4];
    int ghostBlock[4][4];
} GameState;

GameState game;

void initGraphics() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

}

// Menggambar papan permainan
void drawBoard() {
    int x = 100, y = 100;
    // TODO: Implementasi menggambar papan dan blok
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

int main() {
    initGraphics();
    gameLoop();
    closegraph();
    return 0;
}
