#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../header/Rifky.h"
#include "../header/Micky.h"
#include "../header/config.h"

Tetromino findShadowPosition(Tetromino *t, Grid *grid) {
    Tetromino shadow = *t; // Salin tetromino asli

    while (canMoveDown(&shadow, grid)) {
        for (int i = 0; i < 4; i++) {
            shadow.blocks[i].y += 1;
        }
    }
    return shadow;
}

void drawShadowBlock(Tetromino *t, Grid *grid) {
    Tetromino shadow = findShadowPosition(t, grid);

    setcolor(WHITE);  // Gunakan warna gelap untuk bayangan
    setlinestyle(DOTTED_LINE, 0, 1);  // Garis putus-putus untuk efek bayangan

    for (int i = 0; i < 4; i++) {
        int x = shadow.blocks[i].x * BLOCK_SIZE + 220;
        int y = shadow.blocks[i].y * BLOCK_SIZE + 50;

        rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
    }

    setlinestyle(SOLID_LINE, 0, 1); // Kembalikan ke garis solid setelah menggambar
}

void drawLeadPanel(Panel panel) {
    setcolor(WHITE);
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    char leaderBoardText[] = "Leaderboard";
    outtextxy(panel.x + 127, panel.y + 20, leaderBoardText);

    FILE *file = fopen("leaderboard.txt", "r");

    if (file == NULL) {
        outtextxy(panel.x + 20, panel.y + 50, (char*)"No scores available");
        return;
    }

    char name[50];
    int score;
    char timeStr[100];
    int yOffset = 50;
    char scoreEntry[200];

    while (fscanf(file, "%s %d", name, &score) == 2 && fgets(timeStr, sizeof(timeStr), file) != NULL && yOffset < panel.height - 20) {
        // Hapus newline dari fgets
        timeStr[strcspn(timeStr, "\n")] = 0;

        // Hilangkan spasi awal (karena fgets dimulai setelah fscanf)
        char *cleanTime = timeStr;
        while (*cleanTime == ' ') cleanTime++;

        // Gabungkan dan tampilkan
        sprintf(scoreEntry, "%s: %d (%s)", name, score, cleanTime);
        outtextxy(panel.x + 20, panel.y + yOffset, scoreEntry);
        yOffset += 20;
    }

    fclose(file);
}

void saveScoreToFile(const char *username, int score) {
    FILE *file = fopen("leaderboard.txt", "r");
    Scoring *head = NULL;

    // Load existing scores
    if (file != NULL) {
        char tempName[50], tempTime[30];
        int tempScore;
        while (fscanf(file, "%s %d %s", tempName, &tempScore, tempTime) == 3) {
            Scoring *newNode = (Scoring*)malloc(sizeof(Scoring));
            strcpy(newNode->username, tempName);
            newNode->score = tempScore;
            strcpy(newNode->time, tempTime);
            newNode->next = head;
            head = newNode;
        }
        fclose(file);
    }

    // Get current time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timeStr[30];
    strftime(timeStr, sizeof(timeStr), "%Y%m%d_%H%M%S", t);

    // Add new score
    Scoring *newNode = (Scoring*)malloc(sizeof(Scoring));
    strcpy(newNode->username, username);
    newNode->score = score;
    strcpy(newNode->time, timeStr);
    newNode->next = head;
    head = newNode;

    // Sort linked list by score descending
    for (Scoring *i = head; i != NULL; i = i->next) {
        for (Scoring *j = i->next; j != NULL; j = j->next) {
            if (j->score > i->score) {
                // Swap data
                int tempScore = i->score;
                char tempName[50], tempTime[30];
                strcpy(tempName, i->username);
                strcpy(tempTime, i->time);

                i->score = j->score;
                strcpy(i->username, j->username);
                strcpy(i->time, j->time);

                j->score = tempScore;
                strcpy(j->username, tempName);
                strcpy(j->time, tempTime);
            }
        }
    }

    // Save top 10 to file
    file = fopen("leaderboard.txt", "w");
    if (file == NULL) {
        perror("Gagal membuka file untuk ditulis");
        return;
    }

    int count = 0;
    Scoring *curr = head;
    while (curr != NULL && count < 10) {
        fprintf(file, "%s %d %s\n", curr->username, curr->score, curr->time);
        curr = curr->next;
        count++;
    }

    fclose(file);

    // Free linked list
    while (head != NULL) {
        Scoring *temp = head;
        head = head->next;
        free(temp);
    }
}
