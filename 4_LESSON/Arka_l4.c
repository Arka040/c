#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *Reverse(char *array, int len) {
    char *dop_arr = malloc(len);
    int count = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (array[i] == ' ') {
            strncat(dop_arr, &array[i + 1], count);
            count = 0;
        }
        count++;
    }
    if (array[0] != ' ') {
        strncat(dop_arr, array, count);
    }
    dop_arr[len - 1] = '\0';
    return dop_arr;
}


int main (int argc, char **argv) {
    double sum = 0;
    int len = 0;
    for (int i = 1; i < argc; i++) {
        len += strlen(argv[i]);    
    }
    double c = 0;
    char *arr = malloc(len + 1);
    char *s;
    arr[0] = '\0';
    for (int i = 1; i < argc; i++) {
        c = 0;
        c = strtod(argv[i], &s);
        if (*s == '\0') {
            sum += c;
        } else {
            strcat(arr, argv[i]);
        }
    }
    arr[len] = '\0';
    
    printf("--------------------------------------------------------------------------------\n");
    printf("%s\n", arr);
    printf("--------------------------------------------------------------------------------\n");
    printf("%f\n", sum);
    printf("--------------------------------------------------------------------------------\n");

    int i = 0;
    int space_count = 0;
    int stime;  
    long ltime;
    ltime = time(NULL);
    stime = (unsigned int) ltime/2 % 6;
    srand(stime);
    int r;
    char *d = malloc(len + 1);
    char *spc = " ";
    while ((i) < strlen(arr)) {
        space_count++;
        r = rand() % 6;
        printf("%d\n", r);
        d = realloc(d, len + 1 + space_count);
        strncat(d, &arr[i], r);
        strcat(d, spc);
        i += r;
    }
    arr = realloc(arr, len + 1 + space_count);
    arr = d;
    printf("--------------------------------------------------------------------------------\n");
    printf("%s\n", arr);
    int new_len = len + 1 + space_count;

    arr = Reverse(arr, new_len);
    printf("--------------------------------------------------------------------------------\n");
    printf("%s\n", arr);
    printf("--------------------------------------------------------------------------------\n");
    free(arr);
    free(d);

    return 0;
}
