#include "../header/Hafizh.h"
#include "../header/config.h"

// Tetromino nextTetromino;

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

void drawNextTetromino(Tetromino next, int posX, int posY) {
    // inisialisasi blocksize 
    int blockSize = BLOCK_SIZE / 2;

    int offsetx = 30;
    int offsety = 10;
    //set warna fill style dan outline untuk block berikutnya
    setcolor(next.color); 
    setfillstyle(SOLID_FILL, next.color);

    //gambar next block
    for (int i = 0; i < 4; i++) {
        int x = posX + offsetx + next.blocks[i].x * blockSize;
        int y = posY + offsety + next.blocks[i].y * blockSize;
        bar(x, y, x + blockSize, y + blockSize); // Menggambar kotak terisi warna
        rectangle(x, y, x + blockSize, y + blockSize);
    }
}
Tetromino getNewTetromino() {
    Tetromino current = nextTetromino;  // Tetromino yang sebelumnya sudah ditampilkan menjadi aktif
    nextTetromino = createTetromino(rand() % 7, 5, -3); // Buat Tetromino berikutnya
    return current;
}

int setRandomTetromino() {
    srand(time(NULL)); // Seed untuk angka acak
    int type = rand() % 7; // Mencari angka acak dari 1 - 7 
    return type;
}