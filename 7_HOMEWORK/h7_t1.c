#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int File_length(int fd) {
    int n, l = 0;

    while(read(fd, &n, sizeof(int))) {
        l += sizeof(int);
    }
    lseek(fd, 0, SEEK_SET);
    return l;
}

char *int_to_str(int n) {                                 // функция преобрзует число из бинарного файла в строку для записи в текстовый файл
    int k = n, len = 0, d, o, i = 0;
    char *str = malloc(sizeof(char) * 13);
    if (k < 0) {
        i--;
        str[0] = '-';
        n = n - n - n;
    }
    k = n;
    while (k > 0) {
        len++;
        k = k / 10;
    }
    k = n;

    if (k == 0) {
        str[0] = '0';
        str[1] = ' ';
        str[2] = '\0';
        return str;
    }
    str[len - i] = ' ';
    str[len - i + 1] = '\0';
    
    while (k > 0) {
        str[len - 1 - i] = (k % 10) + '0';
        k = k / 10;
        i++;
    }
    return str;
}

int main(int argc, char **argv) {
    int f, num, cur, start = 0, len, end = 0, take, fd;
    f = open(argv[1], O_RDWR);

    struct stat st1, st2;

    if ( (stat(argv[1], &st1) != -1)  && (stat(argv[2], &st2) != -1) ) {     // проверяем одинаковые ли файлы
        if (st1.st_ino == st2.st_ino) {
            printf("Same files\n");
            return 0;
        }
    } else {
        printf("Can not open the file\n");
        return 0;
    }
    
    len = File_length(f);

    // идея моего решения заключается в том, что 
    // 1) Если число отрицательное, то я прыгаю на начало файла + столько чисел, сколько я уже записал в начало и записываю поверх отрицательное число
    // 2) Если число положительное, то я просто прыгаю в конец файла и записываю туда число
    // 3) После того как мы полностью прошлись по длине файла, то мы получим, что в начале стоят отрицательные числа, затем какая-то мешанина
    // до конца длины неизмененного файла, а затем положительные числа. В таком случае прыгаем на начало файла + столько отрицательных чисел, сколько там
    // уже записано и начинаем с этого момента записывать те положительыне числа, которые мы добавили в конце файла

    while(read(f, &num, sizeof(int)) && cur < len) {
        cur = lseek(f, 0, SEEK_CUR);
        if (num < 0) {                                 
            lseek(f, start, SEEK_SET);
            start += sizeof(int);
            write(f, &num, sizeof(int));
            lseek(f, cur, SEEK_SET);
        } else {
            lseek(f, 0, SEEK_END);
            write(f, &num, sizeof(int));
            lseek(f, cur, SEEK_SET);
        }
    }

    lseek(f, len, SEEK_SET);

    while(read(f, &num, sizeof(int)) && start < len) {
        cur = lseek(f, 0, SEEK_CUR);
        lseek(f, start, SEEK_SET);
        start += sizeof(int);
        write(f, &num, sizeof(int));
        lseek(f, cur, SEEK_SET);
    }

    ftruncate(f, len);

    close(f);

    f = open(argv[1], O_RDONLY);                   
    FILE *second_f = fopen(argv[2], "w+"); 
    char *text;
    while (read(f, &num, sizeof(int))) {
        fputs(int_to_str(num), second_f);
        cur = lseek(f, 0, SEEK_CUR);
    }
    close(f);
    fclose(second_f);
    return 0;
}