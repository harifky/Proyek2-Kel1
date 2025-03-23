#include <graphics.h>
#include "header/game.h"
#include "header/grid.h"
#include "header/tetromino.h"

int main() {
    
    playGame();
    
    getch();
    closegraph();
    return 0;
}
