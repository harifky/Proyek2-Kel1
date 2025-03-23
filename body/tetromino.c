#include <graphics.h>
#include <time.h>
#include "../header/config.h"
#include "../header/tetromino.h"
#include "../header/game.h"
#include "../header/grid.h"

Tetromino nextTetromino;

// Definisi bentuk Tetromino (koordinat relatif terhadap pusat rotasi)
const Block tetrominoShapes[7][4] = {
    {{-1, 0}, {0, 0}, {1, 0}, {2, 0}},  // I (Garis)
    {{1, 0}, {0, 0}, {0, 1}, {1, 1}},   // O (Kotak)
    {{-1, 0}, {0, 0}, {1, 0}, {0, 1}},  // T
    {{-1, 0}, {0, 0}, {1, 0}, {1, 1}},  // L
    {{1, 0}, {0, 0}, {-1, 0}, {1, -1}}, // J
    {{1, 0}, {0, 0}, {0, 1}, {-1, 1}},  // Z
    {{-1, 0}, {0, 0}, {0, 1}, {1, 1}}   // S
};

// Warna untuk Tetromino
const int tetrominoColors[7] = {CYAN, YELLOW, MAGENTA, BLUE, LIGHTRED, GREEN, RED};

// Fungsi untuk membuat Tetromino
/*Dibuat oleh Hafizh Andika*/
Tetromino createTetromino(int type, int startX, int startY) {
    //Kamus lokal inisialisasi tetromino sebagai t
    Tetromino t;

    // looping untuk membuat bentuk tetromino
    for (int i = 0; i < 4; i++) {
        t.blocks[i].x = startX + tetrominoShapes[type][i].x;
        t.blocks[i].y = startY + tetrominoShapes[type][i].y;
    }
    t.color = tetrominoColors[type]; // Menentukan warna berdasarkan tipe
    return t;
}

// Fungsi untuk menggambar Tetrominoz
/*Dibuat oleh Hafizh Andika*/
void drawTetromino(Tetromino t) {

    //set warna block
    setfillstyle(SOLID_FILL, t.color);
    
    //looping untuk menggambar bentuk tetromino ke layar
    for (int i = 0; i < 4; i++) {
        int x = t.blocks[i].x * BLOCK_SIZE + 220;  // Koordinat X dalam grid
        int y = t.blocks[i].y * BLOCK_SIZE + 50;   // Koordinat Y dalam grid
        
        // Gambar hanya jika sebagian dari blok masuk layar
        if (t.blocks[i].y >= -5) {  
            bar(x, y, x + BLOCK_SIZE - 2, y + BLOCK_SIZE - 2);

            // Gambar outline blok
            setcolor(WHITE);  // Set warna outline
            rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
        }
    }
}

// fungsi untuk menggerakkan blok tetromino
/*Dibuat oleh Micky Ridho Pratama*/
void moveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {

    //Block dapat digerakkan jika tidak melebihi batas grid
    if (canMoveTetromino(t, grid, dx, dy)) {
        for (int i = 0; i < 4; i++) {
            t->blocks[i].x += dx;
            t->blocks[i].y += dy;
        }
    }
}

//fungsi untuk memutar blok tetromino
/*Dibuat oleh Micky Ridho*/
void rotateTetromino(Tetromino *t, Grid *grid) {
    int pivotX = t->blocks[1].x; // Titik pusat rotasi
    int pivotY = t->blocks[1].y;
    
    Block rotatedBlocks[4];
    
    for (int i = 0; i < 4; i++) {
        int relativeX = t->blocks[i].x - pivotX;
        int relativeY = t->blocks[i].y - pivotY;

        // Rotasi searah jarum jam (90° CW)
        rotatedBlocks[i].x = pivotX - relativeY;
        rotatedBlocks[i].y = pivotY + relativeX;
    }
    
    // Cek batas grid dan geser jika perlu
    int minX = GRID_WIDTH, maxX = 0;
    for (int i = 0; i < 4; i++) {
        if (rotatedBlocks[i].x < minX) minX = rotatedBlocks[i].x;
        if (rotatedBlocks[i].x > maxX) maxX = rotatedBlocks[i].x;
    }
    
    int shiftX = 0;
    if (minX < 0) shiftX = -minX;  // Geser ke kanan jika keluar kiri
    if (maxX >= GRID_WIDTH) shiftX = GRID_WIDTH - 1 - maxX; // Geser ke kiri jika keluar kanan

    // Terapkan pergeseran jika diperlukan
    for (int i = 0; i < 4; i++) {
        rotatedBlocks[i].x += shiftX;
        
        // Cek tabrakan dengan blok lain di grid
        if (rotatedBlocks[i].y >= 0 && grid->cells[rotatedBlocks[i].y][rotatedBlocks[i].x] != 0) {
            return; // Batalkan rotasi jika bertabrakan
        }
    }
    
    // Terapkan rotasi jika valid
    for (int i = 0; i < 4; i++) {
        t->blocks[i] = rotatedBlocks[i];
    }
}

//fungsi untuk mengecek apakah tetromino masih bisa jatuh atau tidak
/*Dibuat oleh Micky Ridho*/
int canMoveDown(Tetromino *t, Grid *grid) {
    for (int i = 0; i < 4; i++) {
        int x = t->blocks[i].x;
        int y = t->blocks[i].y + 1; // Cek sel di bawahnya

        // Jika sudah di dasar grid, tidak bisa turun
        if (y >= GRID_HEIGHT) {
            return 0; 
        }

        // Cek hanya jika sudah masuk dalam area grid
        if (y >= 0 && grid->cells[y][x] != 0) {
            return 0; // Tidak bisa turun karena ada blok lain
        }
    }
    return 1; // Bisa turun
}

//fungsi untuk menjatuhkan tetromino langsung ke bawah
/*Dibuat oleh Micky Ridho*/
void hardDropTetromino(Tetromino *t, Grid *grid, int *score) {
    //block akan langsung dijatuhkan selama condisi canMoveDown)() true
    while (canMoveDown(t, grid)) {
        moveTetromino(t, grid, 0, 1); // Turunkan terus hingga mentok
    }
    
    storeTetrominoInGrid(grid, t); // Simpan blok didalam grid
    // printf("Memeriksa baris penuh...\n"); //?DEBUGGING
    
    *score = addScore(score, grid); //tambahkan score
    
    // Buat Tetromino baru setelah hard drop
    *t = getNewTetromino();
}

//fungsi untuk mengecek apakah tetromino didalam area yang dapat bergerak atau digerakkan
/*Dibuat oleh Micky Ridho*/
int canMoveTetromino(Tetromino *t, Grid *grid, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        int newX = t->blocks[i].x + dx;
        int newY = t->blocks[i].y + dy;
        
        // Cek batas grid
        if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT) {
            return 0; // Kembalikan 0 jika Tidak bisa bergerak
        }

        // Cek tabrakan dengan blok yang sudah tersimpan
        if (newY >= 0 && grid->cells[newY][newX] != 0) {
            return 0; // Kembalikan 0 jika Tidak bisa bergerak
        }
    }
    return 1; // Kembalikan 1 jika bisa bergerak
}

//fungsi untuk menyimpan tetromino didalam grid
/*Dibuat oleh M. Naufal Alfarizky*/
void storeTetrominoInGrid(Grid *grid, Tetromino *t) {
    // printf("Menyimpan tetromino...\n"); //?DEBUGGING

    //Gambar blok dan simpan jika sudah mencapai batas pergerakan
    for (int i = 0; i < 4; i++) {
        int x = t->blocks[i].x;
        int y = t->blocks[i].y;
        
        if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
            grid->cells[y][x] = t->color;
        }
    }

    //?DEBUGGING
    // printf("Grid setelah tetromino disimpan:\n");
    // for (int y = 0; y < GRID_HEIGHT; y++) {
    //     for (int x = 0; x < GRID_WIDTH; x++) {
    //         printf("%d ", grid->cells[y][x]);
    //     }
    //     printf("\n");
    // }
    // printf("Keluar dari storeTetrominoInGrid()\n\n");


}

//fungsi untuk menentukan tetromino secara acak
/*Dibuat oleh M. Naufal alfarizky*/
int setRandomTetromino() {
    srand(time(NULL)); // Seed untuk angka acak
    int type = rand() % 7; // Mencari angka acak dari 1 - 7 
    return type;
}

//fungsi untuk mekanisme pernghitungan skor
/*Dibuat oleh M. Naufal Alfarizky*/
int addScore(int *score, Grid *grid){
    // printf("Memeriksa baris penuh...\n"); //? DEBUGGING

    //Mendapatkan jumlah baris yang dihapus
    int rowsCleared = clearFullRows(grid);
    // printf("Baris dihapus: %d\n", rowsCleared); //? DEBUGGING

    // Tambahkan skor berdasarkan jumlah baris yang dihapus
    if (rowsCleared == 1) {
        *score += 100;
    } else if (rowsCleared == 2) { 
        *score += 250;
    } else if (rowsCleared == 3) {
        *score += 400;
    } else if (rowsCleared == 4) {
        *score += 800;
    }

    return *score;

    // printf("Score setelah update: %d\n", *score); //? Debugging
}

//fungsi untuk menyalin tetromino yang digunakan dalam shadow block
/*Dibuat oleh Rifky Hermawan*/
Tetromino findShadowPosition(Tetromino *t, Grid *grid) {
    Tetromino shadow = *t; // Salin tetromino saat ini

    //shadow block akan ditampilkan selama blok masih bisa jatuh
    while (canMoveDown(&shadow, grid)) {
        for (int i = 0; i < 4; i++) {
            shadow.blocks[i].y += 1;
        }
    }
    return shadow;
}

//fungsi untuk membuat shadow block
/*Dibuat oleh Rifky Hermawan*/
void drawShadowBlock(Tetromino *t, Grid *grid) {
    Tetromino shadow = findShadowPosition(t, grid); //Mencari posisi block yang sedang jatuh

    setcolor(WHITE);  // Set warna untuk shadowblock
    setlinestyle(SOLID_LINE, 0, 1);  // Garis putus-putus untuk efek bayangan

    //Gambar shadowblock
    for (int i = 0; i < 4; i++) {
        int x = shadow.blocks[i].x * BLOCK_SIZE + 220;
        int y = shadow.blocks[i].y * BLOCK_SIZE + 50;

        rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
    }
}

//fungsi untuk menampilkan tetromino di bagian next
/*Dibuat oleh Hafizh Andika*/
void drawNextTetromino(Tetromino next, int posX, int posY) {
    // inisialisasi blocksize 
    int blockSize = BLOCK_SIZE / 2;
    
    //set warna fill style dan outline untuk block berikutnya
    setcolor(next.color); 
    setfillstyle(SOLID_FILL, next.color);

    //gambar next block
    for (int i = 0; i < 4; i++) {
        int x = posX + next.blocks[i].x * blockSize;
        int y = posY + next.blocks[i].y * blockSize;
        bar(x, y, x + blockSize, y + blockSize); // Menggambar kotak terisi warna
        rectangle(x, y, x + blockSize, y + blockSize);
    }
}

//fungsi untuk memastikan tetromino yang muncul di bagian 'next' akan keluar selanjuntya
/*Dibuat oleh M. Naufal Alfarizky*/
Tetromino getNewTetromino() {
    Tetromino current = nextTetromino;  // Tetromino yang sebelumnya sudah ditampilkan menjadi aktif
    nextTetromino = createTetromino(rand() % 7, 5, -3); // Buat Tetromino berikutnya
    return current;
}
