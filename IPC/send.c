#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <math.h>

#define MSGSZ     128

char stage_0[] = "\n";
char stage_1[] = "\n\n\n\n\n________\n";
char stage_2[] = "|\n|\n|\n|\n|\n|_______\n";
char stage_3[] = "|------\n|\n|\n|\n|\n|_______\n";
char stage_4[] = "|------\n|     O\n|\n|\n|\n|_______\n";
char stage_5[] = "|------\n|     O\n|     |\n|\n|\n|_______\n";
char stage_6[] = "|------\n|     O\n|     |\\\n|\n|\n|_______\n";
char stage_7[] = "|------\n|     O\n|    /|\\\n|\n|\n|_______\n";
char stage_8[] = "|------\n|     O\n|    /|\\\n|      \\\n|\n|_______\n";
char stage_9[] = "|------\n|     O\n|    /|\\\n|    / \\\n|\n|_______\n";

char *stages[10] = {stage_0, stage_1, stage_2, stage_3, stage_4, stage_5, stage_6, stage_7, stage_8, stage_9};

char eng_words[][20] = {"street", "partner", "product", "market", "magazine", "challenge", "hotel", "watermelon", "apple", "juice", "orange", "project", "click"};

typedef struct msgbuf {
         long    mtype;
         char    mtext[MSGSZ];
         } message_buf;

struct {
    long mtype;
    char stopFlag;
    char mtext[MSGSZ];
} Messageto;

struct {
    long mtype;
    char stopFlag;
    char mtext[MSGSZ];
} Messagefrom;


int main() {

    char *locale = setlocale(LC_ALL, "");

    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;

    key = ftok("s10.txt", 'a');

    int msgid = msgget(key, msgflg);

    Messageto.mtype = 1;
    char *str = malloc(100);

    printf("Server started!\n");

    strcpy(str, "Game started!");
    strcpy(Messageto.mtext, str);
    msgsnd(msgid, &Messageto, sizeof(Messageto), 0);
    int i = 0;


    srand(time(NULL));   
    int r = rand();  


    int rand_word_index = r % sizeof(eng_words) / 20;
    char *word = eng_words[rand_word_index];

    int word_len = strlen(word);
    char* empty_word = malloc(20);

    // printf("%s\n", word);

    for (int i = 0; i < word_len; i++) {
        empty_word[i] = '_';
    }

    strcpy(Messageto.mtext, empty_word);
    msgsnd(msgid, &Messageto, sizeof(Messageto), 0);

    int find_letter_flag = 0;
    int current_stage = 0;
    char *letter = malloc(20);

    char *used_letters = malloc(30);
    int inputted_letters = 0;
    int used_letters_flag = 0;

    if (current_stage == 9) {
        Messageto.stopFlag = 1;
        strcpy(Messageto.mtext, word);
        msgsnd(msgid, &Messageto, sizeof(Messageto), 1);
    } else {
        msgsnd(msgid, &Messageto, sizeof(Messageto), 0);
    }

    while(1) {

        msgrcv(msgid, &Messagefrom, sizeof(Messagefrom), 2, 0);
        // if (Messagefrom.stopFlag == 1) {
        //     break;
        // }

        inputted_letters++;

        used_letters_flag = 0;

        if (strlen(Messagefrom.mtext) == 1) {
            for (int i = 0; i < word_len; i++) {
                if (word[i] ==  Messagefrom.mtext[0]) {
                    empty_word[i] = word[i];
                    find_letter_flag = 1;
                }
            }

            if (find_letter_flag == 0) {
                current_stage++;
            } else {
                if (strcmp(word, empty_word) == 0) {
                    Messageto.stopFlag = 2;
                    strcpy(Messageto.mtext, word);
                    msgsnd(msgid, &Messageto, sizeof(Messageto), 0);
                    break;
                }
            }
        } else {
            if (strcmp(word, Messagefrom.mtext) == 0) {
                Messageto.stopFlag = 2;
                strcpy(Messageto.mtext, word);
                msgsnd(msgid, &Messageto, sizeof(Messageto), 0);
                break;
            } else {
                current_stage++;
            }
        }

        if (find_letter_flag == 0) {
            strcpy(Messageto.mtext, "Netu takoi bukvi");
            msgsnd(msgid, &Messageto, sizeof(Messageto), 0);
        } else {
            strcpy(Messageto.mtext, "Est' this letter");
            msgsnd(msgid, &Messageto, sizeof(Messageto), 0);
        }

        strcpy(Messageto.mtext, empty_word);
        msgsnd(msgid, &Messageto, sizeof(Messageto), 0);

        strcpy(Messageto.mtext, stages[current_stage]);
        msgsnd(msgid, &Messageto, sizeof(Messageto), 0);

        if (current_stage == 9) {
            Messageto.stopFlag = 1;
            strcpy(Messageto.mtext, word);
            msgsnd(msgid, &Messageto, sizeof(Messageto), 1);
            sleep(1);
            break;
        } else {
            msgsnd(msgid, &Messageto, sizeof(Messageto), 0);
        }

        find_letter_flag = 0;
    }

    msgctl(msgid, IPC_RMID, 0);

    printf("Server closed");
    exit(0);
}