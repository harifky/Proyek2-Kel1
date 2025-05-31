#include "../header/Hafizh.h"
#include "../header/config.h"

// Membuat tetromino baru berdasarkan tipe dan posisi awal
Tetromino createTetromino(int type, int startX, int startY) {
    Tetromino t;
    t.x = startX;
    t.y = startY;

    // Inisialisasi posisi 4 blok dalam tetromino
    for (int i = 0; i < 4; i++) {
        t.blocks[i].x = startX + tetrominoShapes[type][i].x;
        t.blocks[i].y = startY + tetrominoShapes[type][i].y;
    }

    t.color = tetrominoColors[type]; // Tetapkan warna tetromino berdasarkan tipe
    return t;
}

// Menggambar tetromino aktif di dalam grid permainan
void drawTetromino(Tetromino t) {
    setfillstyle(SOLID_FILL, t.color);

    for (int i = 0; i < 4; i++) {
        int x = t.blocks[i].x * BLOCK_SIZE + 400;
        int y = t.blocks[i].y * BLOCK_SIZE + 50;

        // Gambar hanya jika blok berada di dalam layar (y >= -5)
        if (t.blocks[i].y >= -5) {
            bar(x, y, x + BLOCK_SIZE - 2, y + BLOCK_SIZE - 2); // Isi blok
            setcolor(WHITE); // Warna outline
            rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE); // Gambar outline
        }
    }
}

// Menggambar preview tetromino berikutnya di area khusus (box)
void drawNextTetromino(Tetromino next, int boxX, int boxY) {
    int blockSize = BLOCK_SIZE / 2;
    int boxWidth = 160;
    int boxHeight = 80;

    // Hitung batas minimum dan maksimum koordinat blok
    int minX = 999, minY = 999, maxX = -999, maxY = -999;
    for (int i = 0; i < 4; i++) {
        if (next.blocks[i].x < minX) minX = next.blocks[i].x;
        if (next.blocks[i].y < minY) minY = next.blocks[i].y;
        if (next.blocks[i].x > maxX) maxX = next.blocks[i].x;
        if (next.blocks[i].y > maxY) maxY = next.blocks[i].y;
    }

    // Hitung lebar dan tinggi tetromino berdasarkan bounding box
    int tetrominoWidth = (maxX - minX + 1) * blockSize;
    int tetrominoHeight = (maxY - minY + 1) * blockSize;

    // Hitung posisi awal agar tetromino berada di tengah box
    int startX = boxX + (boxWidth - tetrominoWidth) / 2;
    int startY = boxY + (boxHeight - tetrominoHeight) / 2;

    // Gambar setiap blok tetromino
    setcolor(next.color);
    setfillstyle(SOLID_FILL, next.color);
    for (int i = 0; i < 4; i++) {
        int x = startX + (next.blocks[i].x - minX) * blockSize;
        int y = startY + (next.blocks[i].y - minY) * blockSize;

        bar(x, y, x + blockSize, y + blockSize);         // Isi blok
        rectangle(x, y, x + blockSize, y + blockSize);   // Gambar outline
    }
}

// Mengatur tetromino berikutnya sebagai tetromino aktif, dan membuat yang baru untuk next
Tetromino getNewTetromino() {
    Tetromino current = nextTetromino;
    nextTetromino = createTetromino(rand() % 7, 5, -3);
    return current;
}

// Menghasilkan tipe tetromino acak (0–6)
int setRandomTetromino() {
    srand(time(NULL));               // Seed RNG berdasarkan waktu saat ini
    int type = rand() % 7;           // Pilih angka 0–6
    printf("type : %d\n", type);     // Debug: cetak tipe
    return type;
}