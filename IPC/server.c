#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXLEN 256

struct {
    long mtype;
    char stopFlag;
    char Data[MAXLEN];
} Message;

int main (int argc, char ** argv) {
    key_t key;
    int msgid;
    char str[MAXLEN];
    FILE * fr = fopen("file_read.txt", "r");
    key = ftok("file_connecnt.txt", 's');
    msgid = msgget(key, 0666 | IPC_CREAT);

    Message.stopFlag = 'c';
    Message.mtype = 1;
    msgsnd(msgid, &Message, sizeof(Message), 0);
    Message.mtype = 2;
    msgsnd(msgid, &Message, sizeof(Message), 0);

    long i = 1;
    while (fgets(str, MAXLEN, fr)) {
        i += 1;
        Message.mtype = i % 2 + 1;
        strcpy(Message.Data, str);
        msgsnd(msgid, &Message, sizeof(Message), 0);
    }
    fclose(fr);
    Message.stopFlag = 'b';
    Message.mtype = 1;
    msgsnd(msgid, &Message, sizeof(Message), 0);
    Message.mtype = 2;
    msgsnd(msgid, &Message, sizeof(Message), 0);

    msgrcv(msgid, &Message, sizeof(Message), 3, 0);
    msgrcv(msgid, &Message, sizeof(Message), 3, 0);
    
    printf("Server stopped");
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}