#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 800;
const int BLOCK_SIZE = 40;
const int GRID_WIDTH = SCREEN_WIDTH / BLOCK_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / BLOCK_SIZE;

int grid[GRID_WIDTH][GRID_HEIGHT] = {0}; // Grid permainan
int score = 0; // Skor permainan

typedef struct {
    int coords[4][2]; // Koordinat relatif blok
    int color; // Warna blok
} BlockShape;

BlockShape shapes[] = {
    {{{0, 0}, {0, 1}, {0, 2}, {0, 3}}, CYAN},      // Straight
    {{{0, 0}, {1, 0}, {1, 1}, {1, 2}}, BLUE},      // L-shape
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, YELLOW},    // Square
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, GREEN},     // Z-shape
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}}, MAGENTA}    // T-shape
};

// Fungsi menggambar satu blok
void drawBlock(int x, int y, int color) {
    setfillstyle(SOLID_FILL, color);
    bar(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
    rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
}

// Fungsi menggambar latar belakang
void drawBackground() {
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    setcolor(WHITE);
    for (int i = 0; i < SCREEN_WIDTH; i += BLOCK_SIZE)
        line(i, 0, i, SCREEN_HEIGHT);
    for (int i = 0; i < SCREEN_HEIGHT; i += BLOCK_SIZE)
        line(0, i, SCREEN_WIDTH, i);
}

// Fungsi menggambar skor
void drawScore() {
    char scoreText[20];
    setcolor(WHITE);
    sprintf(scoreText, "Score: %d", score);
    outtextxy(10, 10, scoreText);
}

// Fungsi menggambar bentuk blok
void drawShape(int x, int y, BlockShape shape) {
    for (int i = 0; i < 4; i++) {
        drawBlock(x + shape.coords[i][0] * BLOCK_SIZE, y + shape.coords[i][1] * BLOCK_SIZE, shape.color);
    }
}

// Fungsi menggambar grid yang sudah terisi
void drawGrid() {
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            if (grid[i][j] != 0) {
                drawBlock(i * BLOCK_SIZE, j * BLOCK_SIZE, grid[i][j]);
            }
        }
    }
}

// Fungsi memilih blok secara acak
void randomBlock(int &x, int &y, BlockShape &shape) {
    shape = shapes[rand() % (sizeof(shapes) / sizeof(shapes[0]))];
    x = (GRID_WIDTH / 2) * BLOCK_SIZE;
    y = 0;
}

// Fungsi mengecek tabrakan dengan batas atau blok lain
bool checkCollision(int x, int y, BlockShape shape) {
    for (int i = 0; i < 4; i++) {
        int newX = (x / BLOCK_SIZE) + shape.coords[i][0];
        int newY = (y / BLOCK_SIZE) + shape.coords[i][1];

        // Periksa apakah blok berada di dalam batas grid
        if (newY >= GRID_HEIGHT || newX < 0 || newX >= GRID_WIDTH) {
            return true;
        }

        // Periksa apakah ada blok lain di posisi tersebut
        if (grid[newX][newY] != 0) {
            return true;
        }
    }
    return false;
}

// Fungsi menempatkan blok yang sudah jatuh ke grid
void placeBlock(int x, int y, BlockShape shape) {
    for (int i = 0; i < 4; i++) {
        int newX = (x / BLOCK_SIZE) + shape.coords[i][0];
        int newY = (y / BLOCK_SIZE) + shape.coords[i][1];

        // Pastikan tidak menempatkan blok di luar grid
        if (newY < GRID_HEIGHT) {
            grid[newX][newY] = shape.color;
        }
    }
}


// Fungsi menghapus baris penuh dan menambahkan skor
void clearFullRows() {
    for (int j = 0; j < GRID_HEIGHT; j++) {
        bool full = true;
        for (int i = 0; i < GRID_WIDTH; i++) {
            if (grid[i][j] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            score += 100;
            for (int k = j; k > 0; k--) {
                for (int i = 0; i < GRID_WIDTH; i++) {
                    grid[i][k] = (k > 0) ? grid[i][k - 1] : 0;
                }
            }
            
            for (int i = 0; i < GRID_WIDTH; i++) {
                grid[i][0] = 0;
            }
        }
    }
}

// Fungsi untuk memutar blok
void rotateBlock(BlockShape &shape) {
    BlockShape temp = shape;
    for (int i = 0; i < 4; i++) {
        int newX = -shape.coords[i][1];
        int newY = shape.coords[i][0];
        temp.coords[i][0] = newX;
        temp.coords[i][1] = newY;
    }
    shape = temp;
}

// Fungsi untuk menggerakkan blok sesuai input
void moveBlock(int &x, int &y, char key, BlockShape &shape) {
    if (key == 75 && !checkCollision(x - BLOCK_SIZE, y, shape)) x -= BLOCK_SIZE; // Left
    if (key == 77 && !checkCollision(x + BLOCK_SIZE, y, shape)) x += BLOCK_SIZE; // Right
    if (key == 80 && !checkCollision(x, y + BLOCK_SIZE, shape)) y += BLOCK_SIZE; // Down
    if (key == 32) {
        while (!checkCollision(x, y + BLOCK_SIZE, shape)) y += BLOCK_SIZE; // Space to drop
    }
    if (key == 72) {
        BlockShape rotated = shape;
        rotateBlock(rotated);
        if (!checkCollision(x, y, rotated)) shape = rotated;
    }
}

int main() {
    int gd = DETECT, gm;
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris Game");
    srand(time(0));

    int x, y;
    BlockShape currentShape;
    randomBlock(x, y, currentShape);

    while (true) {
        drawBackground();
        drawGrid();
        drawShape(x, y, currentShape);
        drawScore();
        delay(100);

        if (kbhit()) {
            char key = getch();
            if (key == 27) break;
            moveBlock(x, y, key, currentShape);
        }

        if (!checkCollision(x, y + BLOCK_SIZE, currentShape)) {
            y += BLOCK_SIZE;
        } else {
            placeBlock(x, y, currentShape);
            clearFullRows();
            randomBlock(x, y, currentShape);
        }
    }

    closegraph();
    return 0;
}