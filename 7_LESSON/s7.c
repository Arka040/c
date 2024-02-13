#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 250

#include <unistd.h>
#include <sys/types.h>

void f1(char* file, int n) {
    FILE *fin = fopen(file, "r+");
    if (fin == NULL) {
        perror("Ошибка с файлом\n"); //файл не открылся и выводим ошибку которая случилась
        exit(1);
    }
    char mas[MAX_LEN];
    int posw = 0;
    int posr = 0;
    while ( fgets(mas, MAX_LEN, fin) ) {
        if (strlen(mas) <= n) {
            fseek(fin, posw, SEEK_SET);
            fputs(mas, fin);
            posw += strlen(mas);
            posr += strlen(mas);
            fseek(fin, posr, SEEK_SET);
        } 
        else {
            posr += strlen(mas);
            fseek(fin, posr, SEEK_SET);
        }

    }
    ftruncate(fileno(fin), posw);
    fclose(fin);
}

void f2(char* file, int n) {
    FILE *fin = fopen(file, "r+");
    if (fin == NULL) {
        perror("Ошибка с файлом\n"); //файл не открылся и выводим ошибку которая случилась
        exit(1);
    }
    char mas[MAX_LEN*n];
    int posw = 0;
    int posr = 0;
    int size = 0;
    int bufpos = 0;
    fseek(fin, 0, SEEK_SET);

    while (fgets(mas, sizeof(mas), fin)) {
        size += 1;
    }
    char buf[MAX_LEN*(size-1)];
    fseek(fin, 0, SEEK_SET);

    for (int i = 0; i < n; i++) {
        if ( fgets(mas + bufpos, MAX_LEN, fin) ) {
            bufpos += strlen(mas+bufpos);
        }
        else {
            printf("Файл не содержит %d строк\n", n);
            fclose(fin);
            return;
        }
    }
    printf("%s", mas);
    
    bufpos = 0;
    while ( fgets(buf + bufpos, MAX_LEN, fin) ) {
        bufpos += strlen(buf+bufpos);
    }

    fseek(fin, posr, SEEK_SET);
    fputs(buf, fin);
    fputs(mas, fin);

    fclose(fin);
}

    

void f3(char* file, int n) {
    FILE *fin = fopen(file, "r+");
    if (fin == NULL) {
        perror("Ошибка с файлом\n"); //файл не открылся и выводим ошибку которая случилась
        exit(1);
    }
    char mas[MAX_LEN*n];
    int posw = 0;
    int posr = 0;
    int size = 0;
    int bufpos = 0;
    fseek(fin, 0, SEEK_SET);

    while (fgets(mas, sizeof(mas), fin)) {
        size += 1;
    }
    char buf[MAX_LEN*(size-1)];
    fseek(fin, 0, SEEK_SET);

    for (int i = 0; i < size-n; i++) {
        if ( fgets(buf + bufpos, MAX_LEN, fin) ) {
            bufpos += strlen(buf+bufpos);
        }
        else {
            printf("Файл не содержит %d строк\n", n);
            fclose(fin);
            return;
        }
    }
    printf("%s", mas);
    
    bufpos = 0;
    while ( fgets(mas + bufpos, MAX_LEN, fin) ) {
        bufpos += strlen(mas+bufpos);
    }
    //*(mas+bufpos) = '\n';

    fseek(fin, posr, SEEK_SET);
    fputs(mas, fin);
    fputs(buf, fin);

    fclose(fin);
    
}


int main(int argc, char** argv) {
    int funcnum;
    int n;
    void (*function[])(char*, int) = {f1, f2, f3};
    
    printf("Enter num of function, n: ");
    //scanf("%d %d", &funcnum, &n);
    while ( scanf("%d %d", &funcnum, &n) != EOF ) {
        function[funcnum-1](argv[1], n);
    }


    
}
