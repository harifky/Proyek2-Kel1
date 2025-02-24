#include <graphics.h>
#include <conio.h>
#include <stdio.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define BLOCK_SIZE    20

// Struktur untuk menyimpan data permainan
typedef struct {
    int score;
    int level;
    int board[20][10];
} GameState;

GameState game;

void initGraphics() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
}

void drawBlock(int x, int y, int color) {
    setfillstyle(SOLID_FILL, color);
    bar(x, y, x + 20, y + 20);
    rectangle(x, y, x + 20, y + 20);
}

void StarightBlock(int x, int y) {
    for (int i = 0; i < 5; i++)
        drawBlock(x, y + i * 20, CYAN);
}

void ShapeBlock(int x, int y) {
    drawBlock(x, y, BLUE);
    for (int i = 0; i < 3; i++)
        drawBlock(x + 20, y + i * 20, BLUE);
}

void ReverseShapeBlock(int x, int y) {
    for (int i = 0; i < 3; i++)
        drawBlock(x, y + i * 20, BROWN);
    drawBlock(x + 20, y + 40, BROWN);
}

void OBlock(int x, int y) {
    drawBlock(x, y, YELLOW);
    drawBlock(x + 20, y, YELLOW);
    drawBlock(x, y + 20, YELLOW);
    drawBlock(x + 20, y + 20, YELLOW);
}

void RzigzagBlock(int x, int y) {
    drawBlock(x, y + 20, GREEN);
    drawBlock(x + 20, y + 20, GREEN);
    drawBlock(x + 20, y, GREEN);
    drawBlock(x + 40, y, GREEN);
}

void TshapeBlock(int x, int y) {
    for (int i = 0; i < 3; i++)
        drawBlock(x + i * 20, y, MAGENTA);
    drawBlock(x + 20, y + 20, MAGENTA);
}

void LzingzagBlock(int x, int y) {
    drawBlock(x, y, RED);
    drawBlock(x + 20, y, RED);
    drawBlock(x + 20, y + 20, RED);
    drawBlock(x + 40, y + 20, RED);
}

void drawBoard() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (game.board[i][j]) {
                drawBlock(j * BLOCK_SIZE, i * BLOCK_SIZE, game.board[i][j]);
            }
        }
    }
}

void drawScore() {
    char scoreText[20];
    sprintf(scoreText, "Score: %d", game.score);
    outtextxy(SCREEN_WIDTH - 100, 20, scoreText);
}

void gameLoop() {
    while (1) {
        cleardevice();
        drawBoard();
        drawScore();
        StarightBlock(50, 50);
        LzingzagBlock(300, 150);
        ShapeBlock(150, 50);
        ReverseShapeBlock(250, 50);
        OBlock(350, 50);
        RzigzagBlock(450, 50);
        TshapeBlock(550, 50);
        delay(100);
        
        if (kbhit()) {
            char key = getch();
            if (key == 27) break; // ESC untuk keluar
        }
    }
}

int main() {
    initGraphics();
    gameLoop();
    closegraph();
    return 0;
}
