#include <graphics.h>
#include "header/game.h"
#include "header/Naufal_N.h"

int main() {
    initwindow(800, 600, "Tetris Menu"); // Inisialisasi window
    showMenu(); // Panggil menu utama
    closegraph();
    return 0;
}