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
        int x = current->x * BLOCK_SIZE + 220;
        int y = current->y * BLOCK_SIZE + 50;
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
        int x = t.blocks[i].x * BLOCK_SIZE + 220;  // Koordinat X dalam grid
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

    setcolor(next.color); 
    setfillstyle(SOLID_FILL, next.color);

    BlockNode* current = next.head;
    while (current != NULL) {
        int x = posX + current->x * blockSize;
        int y = posY + current->y * blockSize;
        bar(x, y, x + blockSize, y + blockSize);
        rectangle(x, y, x + blockSize, y + blockSize);
        current = current->next;
    }
}*/

void drawNextTetromino(Tetromino next, int posX, int posY) {
    int blockSize = BLOCK_SIZE / 2;
    
    setcolor(next.color);
    setfillstyle(SOLID_FILL, next.color); // Mengatur fill style dan warna
    for (int i = 0; i < 4; i++) {
        int x = posX + next.blocks[i].x * blockSize;
        int y = posY + next.blocks[i].y * blockSize;
        bar(x, y, x + blockSize, y + blockSize); // Menggambar kotak terisi warna
        rectangle(x, y, x + blockSize, y + blockSize);
        // printf("Color: %d\n", next.color);
        // floodfill(x + blockSize, y + blockSize, next.color);
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
    srand(time(NULL)); // Seed untuk angka acak
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
