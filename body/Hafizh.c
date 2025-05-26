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


void drawTetromino(Tetromino t, Grid grid) {
    setfillstyle(SOLID_FILL, t.color);
    BlockNode* current = t.head;
    
    while (current != NULL) {
        int x = current->x * BLOCK_SIZE + grid.x;
        int y = current->y * BLOCK_SIZE + grid.y;
        if (current->y >= -5) {
            bar(x, y, x + BLOCK_SIZE - 2, y + BLOCK_SIZE - 2);
            setcolor(WHITE);
            rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
        }
        current = current->next;
    }
}


void drawNextTetromino(Tetromino next, int posX, int posY, int boxWidth, int boxHeight) {
    int blockSize = BLOCK_SIZE / 2;

    // Hitung bounding box
    int minX = 999, minY = 999, maxX = -999, maxY = -999;
    BlockNode* node = next.head;
    while (node) {
        if (node->x < minX) minX = node->x;
        if (node->y < minY) minY = node->y;
        if (node->x > maxX) maxX = node->x;
        if (node->y > maxY) maxY = node->y;
        node = node->next;
    }

    int tetrominoWidth = (maxX - minX + 1) * blockSize;
    int tetrominoHeight = (maxY - minY + 1) * blockSize;

    int startX = posX + (boxWidth - tetrominoWidth) / 2;
    int startY = posY + (boxHeight - tetrominoHeight) / 2;

    setfillstyle(SOLID_FILL, next.color);
    setcolor(WHITE);

    node = next.head;
    while (node) {
        int x = startX + (node->x - minX) * blockSize;
        int y = startY + (node->y - minY) * blockSize;
        bar(x, y, x + blockSize, y + blockSize);
        rectangle(x, y, x + blockSize, y + blockSize);
        node = node->next;
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
