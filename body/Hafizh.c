#include "../header/Hafizh.h"
#include "../header/config.h"

// Tetromino nextTetromino;

/*Tetromino createTetromino(int type, int startX, int startY) {
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
*/

Tetromino createTetromino(int type, int startX, int startY) {
    Tetromino t;
    t.x = startX;
    t.y = startY;
    for (int i = 0; i < 4; i++) {
        t.blocks[i].x = startX + tetrominoShapes[type][i].x;
        t.blocks[i].y = startY + tetrominoShapes[type][i].y;
    }
    t.color = tetrominoColors[type]; // Menentukan warna berdasarkan tipe
    return t;
}

/*void drawTetromino(Tetromino t) {
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
}*/

void drawTetromino(Tetromino t) {
    // setcolor(t.color);
    setfillstyle(SOLID_FILL, t.color);
    
    for (int i = 0; i < 4; i++) {
        int x = t.blocks[i].x * BLOCK_SIZE + 400;  // Koordinat X dalam grid
        int y = t.blocks[i].y * BLOCK_SIZE + 50;   // Koordinat Y dalam grid
        
        // Gambar hanya jika sebagian dari blok masuk layar
        if (t.blocks[i].y >= -5) {  
            bar(x, y, x + BLOCK_SIZE - 2, y + BLOCK_SIZE - 2);

            // Gambar outline blok
            setcolor(WHITE);  // Ubah warna outline sesuai keinginan
            rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
        }
    }
}


/*void drawNextTetromino(Tetromino next, int posX, int posY) {
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
}*/

void drawNextTetromino(Tetromino next, int boxX, int boxY) {
    int blockSize = BLOCK_SIZE / 2;
    int boxWidth = 160;
    int boxHeight = 80;

    // Hitung bounding box tetromino
    int minX = 999, minY = 999, maxX = -999, maxY = -999;
    for (int i = 0; i < 4; i++) {
        if (next.blocks[i].x < minX) minX = next.blocks[i].x;
        if (next.blocks[i].y < minY) minY = next.blocks[i].y;
        if (next.blocks[i].x > maxX) maxX = next.blocks[i].x;
        if (next.blocks[i].y > maxY) maxY = next.blocks[i].y;
    }

    int tetrominoWidth = (maxX - minX + 1) * blockSize;
    int tetrominoHeight = (maxY - minY + 1) * blockSize;

    // Pusatkan tetromino di tengah box
    int startX = boxX + (boxWidth - tetrominoWidth) / 2;
    int startY = boxY + (boxHeight - tetrominoHeight) / 2;

    setcolor(next.color);
    setfillstyle(SOLID_FILL, next.color);
    for (int i = 0; i < 4; i++) {
        int x = startX + (next.blocks[i].x - minX) * blockSize;
        int y = startY + (next.blocks[i].y - minY) * blockSize;
        bar(x, y, x + blockSize, y + blockSize);
        rectangle(x, y, x + blockSize, y + blockSize);
    }
}

Tetromino getNewTetromino() {
    Tetromino current = nextTetromino;  // Tetromino yang sebelumnya sudah ditampilkan menjadi aktif
    nextTetromino = createTetromino(rand() % 7, 5, -3); // Buat Tetromino berikutnya
    return current;
}

/*int setRandomTetromino() {
    srand(time(NULL)); // Seed untuk angka acak
    int type = rand() % 7; // Mencari angka acak dari 1 - 7 
    return type;
}*/

int setRandomTetromino() {
    // srand(time(NULL)); // Seed untuk angka acak
    int type = rand() % 7;
    printf("type : %d\n", type);
    return type;
}


/*void destroyTetromino(Tetromino* t) {
    BlockNode* current = t->head;
    while (current != NULL) {
        BlockNode* next = current->next;
        free(current);
        current = next;
    }
    t->head = NULL;
}*/
