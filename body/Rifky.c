#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../header/Rifky.h"
#include "../header/Micky.h"
#include "../header/config.h"

/**
 * Menghitung posisi bayangan tetromino (shadow) saat jatuh ke bawah sampai tidak bisa turun lagi.
 * Input: pointer ke tetromino saat ini, pointer ke grid permainan.
 * Output: tetromino shadow yang posisinya paling bawah.
 */
Tetromino findShadowPosition(Tetromino *t, Grid *grid) {
    Tetromino shadow = *t;  // Copy tetromino untuk shadow (deep copy)

    // Geser tetromino ke bawah selama masih bisa bergerak turun
    while (canMoveDown(&shadow, grid)) {
        for (int i = 0; i < 4; i++) {
            shadow.blocks[i].y += 1;
        }
    }

    return shadow;
}

/**
 * Menggambar bayangan tetromino (shadow) dengan garis putus-putus berwarna putih.
 * Input: pointer ke tetromino saat ini dan pointer ke grid.
 */
void drawShadowBlock(Tetromino *t, Grid *grid) {
    Tetromino shadow = findShadowPosition(t, grid);

    setcolor(WHITE);                  // Warna putih untuk shadow
    setlinestyle(DOTTED_LINE, 0, 1); // Garis putus-putus untuk shadow

    for (int i = 0; i < 4; i++) {
        int x = shadow.blocks[i].x * BLOCK_SIZE + 400;  // Offset x sesuai posisi grid
        int y = shadow.blocks[i].y * BLOCK_SIZE + 50;   // Offset y sesuai posisi grid
        rectangle(x, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
    }

    setlinestyle(SOLID_LINE, 0, 1);  // Kembalikan gaya garis ke default (solid)
}

/**
 * Menggambar panel leaderboard di layar.
 * Membaca file leaderboard.txt dan menampilkan username, skor, dan waktu pencapaian.
 * Jika file tidak ada, menampilkan pesan "No scores available".
 */
void drawLeadPanel(Panel panel) {
    setcolor(WHITE);
    rectangle(panel.x, panel.y, panel.x + panel.width, panel.y + panel.height);

    char leaderBoardText[] = "Leader Board";
    outtextxy(panel.x + 50, panel.y + 20, leaderBoardText);

    FILE *file = fopen("Proyek2-Kel1/leaderboard.txt", "r");
    if (file == NULL) {
        outtextxy(panel.x + 20, panel.y + 50, (char*)"No scores available");
        return;
    }

    char name[50];
    int score;
    char timeStr[100];
    int yOffset = 50;
    char scoreEntry[200];

    // Membaca setiap baris dalam format: username score waktu (string)
    while (fscanf(file, "%s %d", name, &score) == 2 && fgets(timeStr, sizeof(timeStr), file) != NULL && yOffset < panel.height - 20) {
        // Hapus newline dari fgets
        timeStr[strcspn(timeStr, "\n")] = 0;

        // Hilangkan spasi awal yang mungkin muncul akibat fgets setelah fscanf
        char *cleanTime = timeStr;
        while (*cleanTime == ' ') cleanTime++;

        // Format dan tampilkan entry leaderboard
        sprintf(scoreEntry, "%s: %d (%s)", name, score, cleanTime);
        outtextxy(panel.x + 20, panel.y + yOffset, scoreEntry);
        yOffset += 20;
    }

    fclose(file);
}

// Menghitung jumlah kemunculan username dalam linked list.
int countName(Scoring *head, const char *username) {
    int count = 0;
    Scoring *curr = head;
    while (curr != NULL) {
        if (strcmp(curr->username, username) == 0) {
            count++;
        }
        curr = curr->next;
    }
    return count;
}

// Mencari skor terendah berdasarkan username dalam linked list.
Scoring* findLowestScoreByName(Scoring *head, const char *username) {
    Scoring *curr = head;
    Scoring *lowest = NULL;

    while (curr != NULL) {
        if (strcmp(curr->username, username) == 0) {
            if (lowest == NULL || curr->score < lowest->score) {
                lowest = curr;
            }
        }
        curr = curr->next;
    }

    return lowest;
}

// Menambahkan skor baru ke linked list.
Scoring* insertScoring(Scoring *head, const char *username, int score, const char *timeStr) {
    Scoring *newNode = (Scoring*)malloc(sizeof(Scoring));
    strcpy(newNode->username, username);
    newNode->score = score;
    strcpy(newNode->time, timeStr);
    newNode->next = head;
    return newNode;  // kepala baru linked list
}

// Menghapus skor berdasarkan username dari linked list.
Scoring* deleteScoring(Scoring *head, const char *username, int score) {
    Scoring *curr = head;
    Scoring *prev = NULL;

    while (curr != NULL) {
        if (strcmp(curr->username, username) == 0 && curr->score == score) {
            if (prev == NULL) {
                Scoring *newHead = curr->next;
                free(curr);
                return newHead;  // node pertama dihapus
            } else {
                prev->next = curr->next;
                free(curr);
                return head;
            }
        }
        prev = curr;
        curr = curr->next;
    }

    return head;  // tidak ada yang dihapus
}

// Menyimpan scoring ke file leaderboard.txt.
void saveScoreToFile(const char *username, int score) {
    FILE *file = fopen("Proyek2-Kel1/leaderboard.txt", "r");
    Scoring *head = NULL;

    // Load file ke linked list
    if (file != NULL) {
        char tempName[50], tempTime[30];
        int tempScore;
        while (fscanf(file, "%s %d %s", tempName, &tempScore, tempTime) == 3) {
            head = insertScoring(head, tempName, tempScore, tempTime);
        }
        fclose(file);
    }

    // Ambil waktu saat ini
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timeStr[30];
    strftime(timeStr, sizeof(timeStr), "%Y%m%d_%H%M%S", t);

    int sameNameCount = countName(head, username);

    if (sameNameCount >= 3) {
        Scoring *lowest = findLowestScoreByName(head, username);
        if (lowest != NULL && score > lowest->score) {
            // Hapus skor terkecil
            head = deleteScoring(head, username, lowest->score);
            // Tambahkan skor baru
            head = insertScoring(head, username, score, timeStr);
        } else {
            return;
        }
    } else {
        head = insertScoring(head, username, score, timeStr);
    }

    // Sorting berdasarkan skor (bubble sort gaya pointer swap bisa dioptimasi lagi)
    for (Scoring *i = head; i != NULL; i = i->next) {
        for (Scoring *j = i->next; j != NULL; j = j->next) {
            if (j->score > i->score) {
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

    // Tulis ulang leaderboard
    file = fopen("Proyek2-Kel1/leaderboard.txt", "w");
    if (file == NULL) {
        perror("Gagal membuka file");
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

    // Bebaskan memori
    while (head != NULL) {
        Scoring *temp = head;
        head = head->next;
        free(temp);
    }
}