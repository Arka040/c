#include <stdio.h>
#include <stdlib.h>
#define N 4

char *Input(char *a) {
    char c;
    int count = 0;
    while (((c = getchar()) != EOF) && (c != '\n')) {
        if (count % 4 == 0) {
            a = realloc(a, N * ((count/4) + 1));
        }
        a[count] = c;
        count++;
    }    
    if (count % 4 == 0) {
        a = realloc(a, N * (count/4 + 1));
    }
    a[count] = '\0';
    return a;
}

int main () {
    char *a = malloc(N);

    a = Input(a);
    
    if (a) {
        printf("%s", a);
    } else {
        printf("Null");
    }
    free(a);

    return 0;
}
