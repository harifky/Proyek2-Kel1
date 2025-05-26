#include "../header/Hafizh.h"
#include "../header/config.h"

// Tetromino nextTetromino;

Tetromino createTetromino(int type, int startX, int startY) {
    Tetromino t;
    t.head = NULL;

    for (int i = 0; i < 4; i++) {
        BlockNode* newBlock = (BlockNode*)malloc(sizeof(BlockNode));
        newBlock->x = startX + tetrominoShapes[type][i].x;
        newBlock->y = startY + tetrominoShapes[type][i].y;
        newBlock->next = t.head;  // prepend ke linked list
        t.head = newBlock;
    }

    t.color = tetrominoColors[type];
    return t;
}


void drawTetromino(Tetromino t) {
    setfillstyle(SOLID_FILL, t.color);
    BlockNode* current = t.head;
    
    while (current != NULL) {
        int x = current->x * BLOCK_SIZE + 220;
        int y = current->y * BLOCK_SIZE + 50;
        if (current->y >= -5) {
            bar(x, y, x + BLOCK_SIZE - 2, y + BLOCK_SIZE - 2);
            setcolor(WHITE);
            rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
        }
        current = current->next;
    }
}


void drawNextTetromino(Tetromino next, int boxX, int boxY, int boxWidth, int boxHeight) {
    int blockSize = BLOCK_SIZE / 2;

    // Hitung ukuran minimum dan maksimum dari posisi block tetromino
    int minX = 10, minY = 10, maxX = -10, maxY = -10;
    BlockNode* curr = next.head;
    while (curr != NULL) {
        if (curr->x < minX) minX = curr->x;
        if (curr->y < minY) minY = curr->y;
        if (curr->x > maxX) maxX = curr->x;
        if (curr->y > maxY) maxY = curr->y;
        curr = curr->next;
    }

    int tetroWidth = (maxX - minX + 1) * blockSize;
    int tetroHeight = (maxY - minY + 1) * blockSize;

    // Hitung posisi tengah dalam kotak next
    int startX = boxX + (boxWidth - tetroWidth) / 2 - minX * blockSize;
    int startY = boxY + (boxHeight - tetroHeight) / 2 - minY * blockSize;

    // Gambar blok
    setcolor(next.color); 
    setfillstyle(SOLID_FILL, next.color);

    BlockNode* current = next.head;
    while (current != NULL) {
        int x = startX + current->x * blockSize;
        int y = startY + current->y * blockSize;
        bar(x, y, x + blockSize, y + blockSize);
        rectangle(x, y, x + blockSize, y + blockSize);
        current = current->next;
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

void destroyTetromino(Tetromino* t) {
    BlockNode* current = t->head;
    while (current != NULL) {
        BlockNode* next = current->next;
        free(current);
        current = next;
    }
    t->head = NULL;
}
