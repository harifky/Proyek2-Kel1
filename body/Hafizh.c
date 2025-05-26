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


// *void drawNextTetromino(Tetromino next, int posX, int posY) {
//     int blockSize = BLOCK_SIZE / 2;

//     // Hitung ukuran minimum dan maksimum dari posisi block tetromino
//     int minX = 10, minY = 10, maxX = -10, maxY = -10;
//     BlockNode* curr = next.head;
//     while (curr != NULL) {
//         if (curr->x < minX) minX = curr->x;
//         if (curr->y < minY) minY = curr->y;
//         if (curr->x > maxX) maxX = curr->x;
//         if (curr->y > maxY) maxY = curr->y;
//         curr = curr->next;
//     }

//     int tetroWidth = (maxX - minX + 1) * blockSize;
//     int tetroHeight = (maxY - minY + 1) * blockSize;

//     // Hitung posisi tengah dalam kotak next
//     int startX = boxX + (boxWidth - tetroWidth) / 2 - minX * blockSize;
//     int startY = boxY + (boxHeight - tetroHeight) / 2 - minY * blockSize;

//     // Gambar blok
//     setcolor(next.color); 
//     setfillstyle(SOLID_FILL, next.color);

//     BlockNode* current = next.head;
//     while (current != NULL) {
//         int x = startX + current->x * blockSize;
//         int y = startY + current->y * blockSize;
//         bar(x, y, x + blockSize, y + blockSize);
//         rectangle(x, y, x + blockSize, y + blockSize);
//         current = current->next;
//     }
// }*/

void drawNextTetromino(Tetromino next, int posX, int posY) {
    const int boxWidth = 80;
    const int boxHeight = 80;

    // Ukuran blok: maksimal muat 4 blok dalam kotak
    int blockSizeX = boxWidth / 4;
    int blockSizeY = boxHeight / 4;
    int blockSize = (blockSizeX < blockSizeY) ? blockSizeX : blockSizeY;

    // Cari bounding box tetromino
    int minX = next.blocks[0].x, maxX = next.blocks[0].x;
    int minY = next.blocks[0].y, maxY = next.blocks[0].y;

    for (int i = 1; i < 4; i++) {
        if (next.blocks[i].x < minX) minX = next.blocks[i].x;
        if (next.blocks[i].x > maxX) maxX = next.blocks[i].x;
        if (next.blocks[i].y < minY) minY = next.blocks[i].y;
        if (next.blocks[i].y > maxY) maxY = next.blocks[i].y;
    }

    int tetroWidth = (maxX - minX + 1) * blockSize;
    int tetroHeight = (maxY - minY + 1) * blockSize;

    // Offset supaya tetromino muncul di tengah kotak 80x80
    int offsetX = posX + (boxWidth - tetroWidth) / 2 - minX * blockSize;
    int offsetY = posY + (boxHeight - tetroHeight) / 2 - minY * blockSize;

    setcolor(next.color);
    setfillstyle(SOLID_FILL, next.color);

    for (int i = 0; i < 4; i++) {
        int x = offsetX + next.blocks[i].x * blockSize;
        int y = offsetY + next.blocks[i].y * blockSize;
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
