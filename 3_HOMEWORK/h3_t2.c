#include <stdio.h>
#include <string.h>
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

char *Find_and_delete(char *full_string, char *find_string, int full_len) {
    int j = 0;
    int k = 0;
    while (full_string[j] != '\0') {
        if (full_string[j] == find_string[k]) {
            k++;
            while ((full_string[j + k] == find_string[k]) && (full_string[j + k] != '\0')) {
                k++;
            }
            
            if (find_string[k] == '\0') {                
                memmove(&full_string[j], &full_string[j + k], (full_len - j - k));
                full_len -= k;
                full_string[full_len] = '\0';
                j -= 1;
            }
            if (full_string[j + k] == '\0') {
                return full_string;
            }
            k = 0;
        }        
        j++;
    }
    return full_string;
}

int main () {
    char *full_string = malloc(N);
    char *find_string = malloc(N);

    full_string = Input(full_string);
    find_string = Input(find_string);

    printf("----------------------------------------------------------\n");

    int i = 0;
    int full_len = 0;
    int find_len = 0;
    while (full_string[i] != '\0') {
        full_len++;
        i++;
    }
    i = 0;
    while (find_string[i] != '\0') {
        find_len++;
        i++;
    }

    int j = 0;
    char *source = " ";
    full_string = Find_and_delete(full_string, find_string, full_len);
    printf("%s\n", full_string);



    return 0;
}