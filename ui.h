#include "ui.h"

void gameLoop() {
    while (1) {
        cleardevice();
        
        drawUI();
        drawBoard();
        drawScore();

        delay(100);
        
        if (kbhit()) {
            char key = getch();
            if (key == 27) break; // ESC untuk keluar
        }
    }
}
