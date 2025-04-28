#include "../header/Rifky.h"
#include "../header/Micky.h"
#include "../header/config.h"

Tetromino findShadowPosition(Tetromino *t, Grid *grid) {
    Tetromino shadow = *t; // Salin tetromino saat ini

    //shadow block akan ditampilkan selama blok masih bisa jatuh
    while (canMoveDown(&shadow, grid)) {
        for (int i = 0; i < 4; i++) {
            shadow.blocks[i].y += 1;
        }
    }
    return shadow;
}

void drawShadowBlock(Tetromino *t, Grid *grid) {
    Tetromino shadow = findShadowPosition(t, grid); //Mencari posisi block yang sedang jatuh

    setcolor(WHITE);  // Set warna untuk shadowblock
    setlinestyle(SOLID_LINE, 0, 1);  // Garis putus-putus untuk efek bayangan

    //Gambar shadowblock
    for (int i = 0; i < 4; i++) {
        int x = shadow.blocks[i].x * BLOCK_SIZE + 220;
        int y = shadow.blocks[i].y * BLOCK_SIZE + 50;

        rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
    }
}

void drawLeadPanel(Panel panel) {
    
    setcolor(WHITE); // set warna untuk panel

    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height); // gambar panel
    
    //Tampilkan teks Leaderboard
    char leaderBoardText[] = "Leader Board";
    outtextxy(panel.x + 50, panel.y + 20, leaderBoardText);

    //Buka file 
    FILE *file = fopen("leaderboard.txt", "r");

    //jika File NULL tampilkan No scores available
    if (file == NULL) {
        char noScoresText[] = "No scores available";  
        outtextxy(panel.x + 20, panel.y + 50, noScoresText);
        return;
    }

    //Tampilkan Leaderboard
    char name[20];
    int score, yOffset = 50;
    while (fscanf(file, "%s %d", name, &score) != EOF && yOffset < panel.height - 20) {
        char scoreEntry[30];
        sprintf(scoreEntry, "%s: %d", name, score);
        outtextxy(panel.x + 20, panel.y + yOffset, scoreEntry);
        yOffset += 20;
    }

    //Tutup file
    fclose(file);
}

void saveScoreToFile(const char *username, int score) {
    FILE *file = fopen("leaderboard.txt", "r");
    char names[100][20];  // simpan lebih banyak dulu
    int scores[100];
    int count = 0;
    int found = 0;

    if (file != NULL) {
        char tempName[20];
        int tempScore;
        while (fscanf(file, "%s %d", tempName, &tempScore) != EOF) {
            if (strcmp(tempName, username) == 0) {
                if (score > tempScore) {
                    tempScore = score;
                }
                found = 1;
            }
            strcpy(names[count], tempName);
            scores[count] = tempScore;
            count++;
        }
        fclose(file);
    }

    if (!found) {
        strcpy(names[count], username);
        scores[count] = score;
        count++;
    }

    // Urutkan descending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {
                int tempScore = scores[j];
                char tempName[20];
                strcpy(tempName, names[j]);

                scores[j] = scores[j + 1];
                strcpy(names[j], names[j + 1]);

                scores[j + 1] = tempScore;
                strcpy(names[j + 1], tempName);
            }
        }
    }

    file = fopen("leaderboard.txt", "w");
    if (file == NULL) {
        perror("Tidak bisa menulis file leaderboard");
        return;
    }

    for (int i = 0; i < count && i < MAX_LEADERBOARD; i++) {
        fprintf(file, "%s %d\n", names[i], scores[i]);
    }

    fclose(file);
}
