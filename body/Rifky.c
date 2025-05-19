#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../header/Rifky.h"
#include "../header/Micky.h"
#include "../header/config.h"

Tetromino findShadowPosition(Tetromino *t, Grid *grid) {
    // Salin linked list tetromino
    Tetromino shadow;
    shadow.color = t->color;
    shadow.head = NULL;

    BlockNode *current = t->head;
    BlockNode *prev = NULL;

    while (current != NULL) {
        BlockNode *newNode = (BlockNode *)malloc(sizeof(BlockNode));
        newNode->x = current->x;
        newNode->y = current->y;
        newNode->next = NULL;

        if (prev == NULL) {
            shadow.head = newNode;
        } else {
            prev->next = newNode;
        }
        prev = newNode;
        current = current->next;
    }

    // Selama masih bisa turun, geser semua blok ke bawah
    while (canMoveDown(&shadow, grid)) {
        BlockNode *node = shadow.head;
        while (node != NULL) {
            node->y += 1;
            node = node->next;
        }
    }

    return shadow;
}

void drawShadowBlock(Tetromino *t, Grid *grid) {
    Tetromino shadow = findShadowPosition(t, grid);

    setcolor(WHITE);  // Set warna putih untuk bayangan
    setlinestyle(DOTTED_LINE, 0, 1);  // Gaya garis putus-putus

    BlockNode *node = shadow.head;
    while (node != NULL) {
        int x = node->x * BLOCK_SIZE + 220;
        int y = node->y * BLOCK_SIZE + 50;
        rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
        node = node->next;
    }

    // Bebaskan memori dari shadow
    node = shadow.head;
    while (node != NULL) {
        BlockNode *temp = node;
        node = node->next;
        free(temp);
    }

    setlinestyle(SOLID_LINE, 0, 1);  // Kembalikan garis ke default jika perlu
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

    char name[50], time[30];
    int score, yOffset = 50;
    char scoreEntry[100];

    while (fscanf(file, "%s %d %s", name, &score, time) == 3 && yOffset < panel.height - 20) {
        sprintf(scoreEntry, "%s: %d (%s)", name, score, time);
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
