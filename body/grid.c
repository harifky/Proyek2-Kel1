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

//fungsi untuk menggambar Grid
/*Dibuat oleh M. Naufal Nurmaryadi*/
void drawGrid(Grid grid) {
    rectangle(grid.x, grid.y, grid.x + grid.width, grid.y + grid.height);
}

// Fungsi untuk menggambar Hold Panel dan menampilkan tetromino yang di-hold
/*Dibuat oleh M. Naufal Nurmaryadi*/
void drawHoldPanel(Panel panel) {
    // 1. Gambar kotak Hold Box
    rectangle(holdBox.x, holdBox.y, holdBox.x + holdBox.width, holdBox.y + holdBox.height);

    char holdText[] = "Hold";
    outtextxy(holdBox.x + 30, holdBox.y + 10, holdText);

    // 2. Jika ada Tetromino yang sedang di-hold
    if (isHolding) {
        int blockSize = BLOCK_SIZE / 2; // Ukuran Tetromino lebih kecil untuk Hold Box

        // 3. Tetapkan posisi tetap untuk pusat Hold Box
        int centerX = holdBox.x + holdBox.width / 2;
        int centerY = holdBox.y + holdBox.height / 2;

        // 4. Cari batas minimum dan maksimum untuk menyusun ulang blok
        int minX = heldTetromino.blocks[0].x;
        int minY = heldTetromino.blocks[0].y;
        int maxX = heldTetromino.blocks[0].x;
        int maxY = heldTetromino.blocks[0].y;

        for (int i = 1; i < 4; i++) {
            if (heldTetromino.blocks[i].x < minX) minX = heldTetromino.blocks[i].x;
            if (heldTetromino.blocks[i].y < minY) minY = heldTetromino.blocks[i].y;
            if (heldTetromino.blocks[i].x > maxX) maxX = heldTetromino.blocks[i].x;
            if (heldTetromino.blocks[i].y > maxY) maxY = heldTetromino.blocks[i].y;
        }

        // 5. Hitung offset agar Tetromino selalu berada di tengah
        int offsetX = (minX + maxX) / 2;
        int offsetY = (minY + maxY) / 2;

        // 6. Gambar Tetromino di dalam Hold Box
        for (int i = 0; i < 4; i++) {
            int bx = centerX + (heldTetromino.blocks[i].x - offsetX) * blockSize;
            int by = centerY + (heldTetromino.blocks[i].y - offsetY) * blockSize;

            setfillstyle(SOLID_FILL, heldTetromino.color);
            bar(bx, by, bx + blockSize, by + blockSize);
            rectangle(bx, by, bx + blockSize, by + blockSize);
        }
    }
}

// fungsi untuk fitur hold block
/*Dibuat oleh M. Naufal Nurmaryadi*/
void holdTetromino(Tetromino *current) {
    static int lastX = 5, lastY = 0;
    Tetromino temp = *current;

    if (!isHolding) {
        heldTetromino = *current;
        isHolding = 1;
        *current = createTetromino(setRandomTetromino(), 5, 0);
    } else {
        Tetromino swapTetromino = heldTetromino;
        heldTetromino = temp;
        \
        *current = swapTetromino;
        current->x = lastX;
        current->y = lastY;
    }
\
    lastX = temp.x;
    lastY = temp.y;
}


// Fungsi untuk menggambar panel informasi (skor, speed, dll.)
/*Dibuat oleh M. Naufal Nurmaryadi*/
void drawPanel(Panel panel, int *score) {
    // gambar panel
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);
    
    // Tampilkan Teks Score
    char scoreText[20];
    sprintf(scoreText, "Score: %d", *score);
    outtextxy(panel.x + 20, panel.y + 20, scoreText);
    
    
    
    // Semakin tinggi level jatuhnya block semakin cepat
    int levelSpeed = 1;
    if (*score >= 4000) {
        levelSpeed = 4;
    } else if (*score >= 2000) {
        levelSpeed = 3;
    } else if (*score >= 1000) {
        levelSpeed = 2; 
    }
    // Tampilkan teks level
    char speedText[] = "Speed";
    char levelSpeedText[10];
    sprintf(levelSpeedText, "Speed: %d", levelSpeed); 
    outtextxy(panel.x + 20, panel.y + 80, levelSpeedText);
    
    
    
    
    // Tampilkan teks next
    char nextText[] = "Next";
    outtextxy(panel.x + 20, panel.y + 150, nextText);
    // Gambar next block
    drawNextTetromino(nextTetromino, panel.x - 60, panel.y + 240);
}

// Fungsi untuk menggambar panel leaderboard
/*Dibuat oleh Rifky Hermawan*/
void drawLeadPanel(Panel panel) {
    
    setcolor(WHITE); // set warna untuk panel

    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height); // gambar panel
    
    //Tampilkan teks Leaderboard
    char leaderBoardText[] = "Leader Board";
    outtextxy(panel.x + 50, panel.y + 20, leaderBoardText);

    //Buka file 
    FILE *file = fopen("leaderboard.txt", "r");

    //jika File NULL tampilkan No scores available
    if (file == NULL) {
        char noScoresText[] = "No scores available";  
        outtextxy(panel.x + 20, panel.y + 50, noScoresText);
        return;
    }

    //Tampilkan Leaderboard
    char name[20];
    int score, yOffset = 50;
    while (fscanf(file, "%s %d", name, &score) != EOF && yOffset < panel.height - 20) {
        char scoreEntry[30];
        sprintf(scoreEntry, "%s: %d", name, score);
        outtextxy(panel.x + 20, panel.y + yOffset, scoreEntry);
        yOffset += 20;
    }

    //Tutup file
    fclose(file);
}


// Fungsi untuk menggambar blok yang sudah tersimpan dalam grid
/*Dibuat oleh M. Naufal Alfarizky*/
void drawStoredBlocks(Grid *grid) {

    //Gambar semua block yang sudah didimpan didalam grid
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
/*Dibuat oleh M. Naufal Alfarizky*/
int clearFullRows(Grid *grid) {
    // Inisialisasi variabel untuk menghitung jumlah baris yang dihapus
    int rowsCleared = 0;

    // Loop setiap baris di grid dari atas ke bawah
    for (int y = 0; y < GRID_HEIGHT; y++) {
        int full = 1;  // Anggap baris ini penuh (flag)

        // Loop melalui setiap kolom di baris saat ini
        for (int x = 0; x < GRID_WIDTH; x++) {
            // Jika ada sel yang kosong (nilai 0), baris ini tidak penuh
            if (grid->cells[y][x] == 0) {
                full = 0; // Set flag menjadi 0
                break;    // Keluar dari loop kolom
            }
        }

        // Jika baris penuh full(flag), hapus baris tersebut
        if (full) {
            // Geser semua baris di atas baris yang penuh ke bawah
            for (int i = y; i > 0; i--) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    // Salin nilai dari baris di atas ke baris saat ini
                    grid->cells[i][j] = grid->cells[i - 1][j];
                }
            }

            // Set baris paling atas (baris 0) menjadi kosong (nilai 0)
            for (int j = 0; j < GRID_WIDTH; j++) {
                grid->cells[0][j] = 0;
            }

            // Tambahkan jumlah baris yang dihapus
            rowsCleared++;
        }
    }

    // Kembalikan jumlah baris yang dihapus
    return rowsCleared;
}

// fungsi untuk memulai backsound
/*Dibuat oleh M. Naufal Alfarizky*/
void playSoundEffect(const char* soundFile) {
    PlaySound(soundFile, NULL, SND_LOOP | SND_ASYNC);
}

// fungsi untuk menghentikan backsound
/*Dibuat oleh M. Naufal Alfarizky*/
void stopSound() {
    PlaySound(NULL, NULL, 0);
}

//fungsi untuk logika menampilkan game over
/*Dibuat oleh M. Naufal Alfarizky*/
int isGameOver(Grid* grid) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        if (grid->cells[0][x] != 0) {
            return 1; // Game Over
        }
    }
    return 0; // Game masih berjalan
}

//fungsi untuk membuat tampilan game over
/*Dibuat oleh M. Naufal Alfarizky*/
void drawGameOverScreen(Grid grid, int score) {
    setcolor(WHITE);
    rectangle(grid.x, grid.y, grid.x + grid.width, grid.y + grid.height);
    
    settextstyle(10, HORIZ_DIR, 5);
    char gameOverText[10] = "GAME OVER";
    outtextxy(300, 200, gameOverText);
    
    
    settextstyle(10, HORIZ_DIR, 3);
    char scoreText[30];
    sprintf(scoreText, "Final Score: %d", score);
    outtextxy(320, 300, scoreText);
    
    // **Minta Username**
    settextstyle(10, HORIZ_DIR, 3);
    char username[20];
    char enterNameText[] = "Enter your name:";
    outtextxy(280, 350, enterNameText);

    int i = 0;
    while (1) {
        char key = getch();
        if (key == 13) {
            settextstyle(10, HORIZ_DIR, 2);
            char exitText[] = "Press enter to exit...";
            outtextxy(270, 430, exitText);
            break;
        }  // Jika Enter ditekan, selesai
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
    
    // Tunggu input sebelum keluar
    getch();
}


// Fungsi untuk menyimpan skor ke dalam file leaderboard.txt
// Fungsi untuk menyimpan skor ke dalam leaderboard
/*Dibuat oleh Rifky Hermawan*/
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