#include <graphics.h>
#include <string.h>
#include <stdio.h>
#include "../header/grid.h"
#include "../header/game.h"
#include "../header/tetromino.h"

#define MAX_LEADERBOARD 5

Tetromino heldTetromino;
int isHolding = 0;
int hasHeldThisTurn = 0;

// Struktur untuk menyimpan informasi tentang kotak Hold
typedef struct {
    int x, y, width, height;
} HoldBox;

HoldBox holdBox = {100, 50, 100, 100}; // Posisi dan ukuran kotak Hold

// Fungsi untuk menggambar Grid
// void drawGrid(Grid grid) {
//     for (int i = 0; i <= GRID_WIDTH; i++) {
//         line(grid.x + i * BLOCK_SIZE, grid.y, grid.x + i * BLOCK_SIZE, grid.y + GRID_HEIGHT * BLOCK_SIZE);
//     }
//     for (int i = 0; i <= GRID_HEIGHT; i++) {
//         line(grid.x, grid.y + i * BLOCK_SIZE, grid.x + GRID_WIDTH * BLOCK_SIZE, grid.y + i * BLOCK_SIZE);
//     }
// }

void drawGrid(Grid grid) {

    rectangle(grid.x, grid.y, grid.x + grid.width, grid.y + grid.height);
    
}
// Fungsi untuk menggambar Hold Panel dan menampilkan tetromino yang di-hold

void drawHoldPanel(Panel panel) {
    // Gambar kotak Hold
    rectangle(holdBox.x, holdBox.y, holdBox.x + holdBox.width, holdBox.y + holdBox.height);
    char holdText[] = "Hold";
    outtextxy(holdBox.x + 30, holdBox.y + 10, holdText);
    // Jika ada tetromino yang di-hold
    if (isHolding) {
        int minX = 5, minY = 5, maxX = -5, maxY = -5;  // Inisialisasi nilai batas
        for (int i = 0; i < 4; i++) {
            if (heldTetromino.blocks[i].x < minX) minX = heldTetromino.blocks[i].x;
            if (heldTetromino.blocks[i].y < minY) minY = heldTetromino.blocks[i].y;
            if (heldTetromino.blocks[i].x > maxX) maxX = heldTetromino.blocks[i].x;
            if (heldTetromino.blocks[i].y > maxY) maxY = heldTetromino.blocks[i].y;
        }

        int tetrominoWidth = (maxX - minX + 1) * (BLOCK_SIZE / 2);
        int tetrominoHeight = (maxY - minY + 1) * (BLOCK_SIZE / 2);

        int startX = holdBox.x + (holdBox.width - tetrominoWidth) / 2;
        int startY = holdBox.y + (holdBox.height - tetrominoHeight) / 2;

        for (int i = 0; i < 4; i++) {
            int bx = startX + (heldTetromino.blocks[i].x - minX) * (BLOCK_SIZE / 2);
            int by = startY + (heldTetromino.blocks[i].y - minY) * (BLOCK_SIZE / 2);

            setfillstyle(SOLID_FILL, heldTetromino.color);
            bar(bx, by, bx + BLOCK_SIZE / 2, by + BLOCK_SIZE / 2);
        }
    }
}

void holdTetromino(Tetromino *current) {
    Tetromino temp = *current;

    if (!isHolding) {// Jika Hold Box kosong
        heldTetromino = *current;
        isHolding = 1;
        *current = createTetromino(setRandomTetromino(), 5, 0);
    } else {// Jika sudah ada tetromino di Hold Box, tukar
        *current = heldTetromino;
        heldTetromino = temp;
        current->x = 5;
        current->y = 0;
    }
}


// Fungsi untuk menggambar panel informasi (skor, speed, dll.)
void drawPanel(Panel panel, int *score) {
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    
    char scoreText[20];
    sprintf(scoreText, "Score: %d", *score);
    
    char speedText[] = "Speed";
    char nextText[] = "Next";
    int levelSpeed = 1;
    

    if (*score >= 4000) {
       levelSpeed = 4;
    } else if (*score >= 2000) {
        levelSpeed = 3;
    } else if (*score >= 1000) {
        levelSpeed = 2; 
    }

    char levelSpeedText[10];
    sprintf(levelSpeedText, "Speed: %d", levelSpeed); 
    
    
    outtextxy(panel.x + 20, panel.y + 20, scoreText);
    outtextxy(panel.x + 20, panel.y + 150, nextText);
    outtextxy(panel.x + 20, panel.y + 80, levelSpeedText);
    
    drawNextTetromino(nextTetromino, panel.x - 60, panel.y + 240);
}

void drawLeadPanel(Panel panel) {
    setcolor(WHITE);
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    
    char leaderBoardText[] = "Leader Board";
    outtextxy(panel.x + 50, panel.y + 20, leaderBoardText);

    FILE *file = fopen("leaderboard.txt", "r");
    if (file == NULL) {
        outtextxy(panel.x + 20, panel.y + 50, "No scores available");
        return;
    }

    char name[20];
    int score, yOffset = 50;
    while (fscanf(file, "%s %d", name, &score) != EOF && yOffset < panel.height - 20) {
        char scoreEntry[30];
        sprintf(scoreEntry, "%s: %d", name, score);
        outtextxy(panel.x + 20, panel.y + yOffset, scoreEntry);
        yOffset += 20;
    }

    fclose(file);
}


// Fungsi untuk menggambar blok yang sudah tersimpan dalam grid
void drawStoredBlocks(Grid *grid) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid->cells[y][x] != 0) {
                setfillstyle(SOLID_FILL, grid->cells[y][x]);
                bar(grid->x + x * BLOCK_SIZE, grid->y + y * BLOCK_SIZE, 
                    grid->x + (x + 1) * BLOCK_SIZE - 2, grid->y + (y + 1) * BLOCK_SIZE - 2);

                setcolor(WHITE);
                rectangle(grid->x + x * BLOCK_SIZE, grid->y + y * BLOCK_SIZE, 
                          grid->x + (x + 1) * BLOCK_SIZE, grid->y + (y + 1) * BLOCK_SIZE);
            }
        }
    }
}

// Fungsi untuk menghapus baris yang sudah penuh
int clearFullRows(Grid *grid) {
    int rowsCleared = 0;
    
    for (int y = 0; y < GRID_HEIGHT; y++) {
        int full = 1;  // Anggap penuh
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid->cells[y][x] == 0) {
                full = 0; // Baris ini tidak penuh
                break;
            }
        }

        if (full) {
            for (int i = y; i > 0; i--) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    grid->cells[i][j] = grid->cells[i - 1][j];
                }
            }

            for (int j = 0; j < GRID_WIDTH; j++) {
                grid->cells[0][j] = 0;
            }

            rowsCleared++; // Tambah jumlah baris yang dihapus
        }
    }

    return rowsCleared;
}

void playSoundEffect(const char* soundFile) {
    PlaySound(soundFile, NULL, SND_LOOP | SND_ASYNC);
}

void stopSound() {
    PlaySound(NULL, NULL, 0);
}

// Mengecek apakah ada blok di baris paling atas
int isGameOver(Grid* grid) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        if (grid->cells[0][x] != 0) {
            return 1; // Game Over
        }
    }
    return 0; // Game masih berjalan
}

void drawGameOverScreen(Grid grid, int score) {
    // cleardevice();
    setcolor(WHITE);
    rectangle(grid.x, grid.y, grid.x + grid.width, grid.y + grid.height);
    
    settextstyle(10, HORIZ_DIR, 5);
    char gameOverText[] = "GAME OVER";
    outtextxy(300, 200, gameOverText);
    
    // settextstyle(10, HORIZ_DIR, 2);
    // char exitText[] = "Press any key to exit...";
    // outtextxy(290, 250, exitText);
    
    settextstyle(10, HORIZ_DIR, 3);
    char scoreText[30];
    sprintf(scoreText, "Final Score: %d", score);
    outtextxy(320, 300, scoreText);
    
    // **Minta Username**
    settextstyle(10, HORIZ_DIR, 3);
    char username[20];
    outtextxy(280, 350, "Enter your name: ");

    int i = 0;
    while (1) {
        char key = getch();
        if (key == 13) break;  // Jika Enter ditekan, selesai
        if (key == 8 && i > 0) { // Jika Backspace ditekan
            i--;
            username[i] = '\0';
        } else if (i < 19) { // Tambahkan karakter ke username
            username[i++] = key;
            username[i] = '\0';
        }

        // Tampilkan input username
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, BLACK);
        bar(280, 370, 480, 400); // Bersihkan area input
        outtextxy(280, 370, username);
    }

    username[i] = '\0'; // Pastikan string diakhiri dengan null terminator

    // **Simpan Skor ke File**
    saveScoreToFile(username, score);
    
    // getch(); // Tunggu input sebelum keluar
}


// Fungsi untuk menyimpan skor ke dalam file leaderboard.txt
// Fungsi untuk menyimpan skor ke dalam leaderboard
void saveScoreToFile(const char *username, int score) {
    FILE *file = fopen("leaderboard.txt", "r");
    char names[MAX_LEADERBOARD][20];
    int scores[MAX_LEADERBOARD];
    int count = 0;
    int found = 0;

    // **1. Baca leaderboard yang ada**
    if (file != NULL) {
        while (fscanf(file, "%s %d", names[count], &scores[count]) != EOF && count < MAX_LEADERBOARD) {
            if (strcmp(names[count], username) == 0) {
                if (score > scores[count]) {
                    scores[count] = score;  // Perbarui skor jika lebih besar
                }
                found = 1;  // Tandai bahwa username sudah ada
            }
            count++;
        }
        fclose(file);
    }

    // **2. Jika username belum ada, tambahkan ke daftar**
    if (!found && count < MAX_LEADERBOARD) {
        strcpy(names[count], username);
        scores[count] = score;
        count++;
    }

    // **3. Urutkan skor dari yang terbesar ke terkecil (Bubble Sort)**
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {
                int tempScore = scores[j];
                char tempName[20];
                strcpy(tempName, names[j]);

                scores[j] = scores[j + 1];
                strcpy(names[j], names[j + 1]);

                scores[j + 1] = tempScore;
                strcpy(names[j + 1], tempName);
            }
        }
    }

    // **4. Simpan hanya 5 skor tertinggi**
    file = fopen("leaderboard.txt", "w");
    if (file == NULL) {
        printf("Error: Tidak dapat membuka file leaderboard.txt untuk menulis\n");
        return;
    }

    for (int i = 0; i < count && i < MAX_LEADERBOARD; i++) {
        fprintf(file, "%s %d\n", names[i], scores[i]);
    }

    fclose(file);
}