#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define MAXLEN 100

int Print(int s, int num, int lineum, FILE* F) {
    if (!F) {
        printf("No such file\n");
        return 0;
    }
    int empty_flag = 0, counter = 0;
    char *str = malloc(MAXLEN);
    while (counter != num && fgets(str, MAXLEN, F)) {
        if (str[0] == '\n') {
            if (!empty_flag) {
                empty_flag++;
                printf("%s", str);
            } else {
                if (!s) {
                    printf("%s", str);
                }
            }
        } else {
            printf("%s", str);
            empty_flag = 0;
        }
        counter++;
    }
    if (counter == num) {
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char** argv) {

static struct termios oldt, newt;

tcgetattr(STDIN_FILENO, &oldt); // сохранение текущего режима

newt = oldt;

newt.c_lflag &= ~(ICANON);

tcsetattr(STDIN_FILENO, TCSANOW, &newt); // изменение

int s_check = 0;
int num = 10;
int lineum = 0;

FILE *F = fopen(argv[argc - 1], "r");
if (!F) {
    printf("No such file\n");
    return 0;
}

if (argc > 2) {
    if (argc == 3) {
        if (argv[1][0] == '-' && argv[1][1] == 's') {
            s_check = 1;
        } else if (argv[1][0] == '-') {
            int i = 1;
            num = 0;
            while (argv[1][i]) {
                num = num * 10 + (argv[1][i] - '0');
                i++;
            }
        } else {
            int i = 1;
            lineum = 0;
            while (argv[1][i]) {
                lineum = lineum * 10 + (argv[1][i] - '0');
                i++;
            }
        }
    }
    else if (argc == 4) {
        if (argv[1][0] == '-' && argv[1][1] == 's') {
            s_check = 1;
        } else {
            int i = 1;
            num = 0;
            while (argv[1][i]) {
                num = num * 10 + (argv[1][i] - '0');
                i++;
            }
        }
        if (argv[2][0] == '-') {
            int i = 1;
            num = 0;
            while (argv[2][i]) {
                num = num * 10 + (argv[2][i] - '0');
                i++;
            }
        } else {
            int i = 1;
            lineum = 0;
            while (argv[2][i]) {
                lineum = lineum * 10 + (argv[2][i] - '0');
                i++;
            }
        }
    }
    else {
        s_check = 1;
        int i = 1;
        num = 0;
        while (argv[2][i]) {
            num = num * 10 + (argv[2][i] - '0');
            i++;
        }
        i = 1;
        lineum = 0;
        while (argv[3][i]) {
            lineum = lineum * 10 + (argv[3][i] - '0');
            i++;
        }
    }
}

printf("%d\n", num);

char *st = malloc(MAXLEN);
for (int i = 1; i < lineum; i++) {
    fgets(st, MAXLEN, F);
}

int res;
char c;
while (c != 'q') {
    if(!Print(s_check, num, lineum, F)) {
        printf("\n");
        fclose(F);
        return 0;
    }
    c = getchar();
    // printf("\n");
    printf("==%c\n", c);
}


tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // восстановление исходного режима

return 0;
}