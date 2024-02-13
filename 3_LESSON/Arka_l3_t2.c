#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 4

char *Input_with_change(char *a) {
    char c;
    int count = 0;
    int space_flag = 0;
    int first_word_flag = 0;
    int start_first_word = 0;   
    int end_first_word = 0;
    int start_last_word = 0;
    int end_last_word = 0;
    int start_word_flag = 0;
    int end_lst_wrd = 0;
    int nachalo_flag = 0;
    while (((c = getchar()) != EOF) && (c != '\n')) {
        if (count % 4 == 0) {
            a = realloc(a, N * ((count/4) + 1));
        }
        if ((c == ' ') && (first_word_flag == 0) && (nachalo_flag == 1)) {
            end_first_word = count;
            first_word_flag = 1;
        }
        if ((c != ' ') && (start_word_flag == 0)) {
            start_last_word = count;
            start_word_flag = 1;
        }
        if ((c == ' ') && (end_lst_wrd == 0)){
            end_last_word = count;
            end_lst_wrd = 1;
        }
        if ((c != ' ') && (nachalo_flag == 0)) {
            start_first_word = count;
            nachalo_flag = 1;
        }
        if ((c != ' ') || (space_flag == 0)) {
            a[count] = c;
            count++;
        }
        if (c != ' ') {
            end_lst_wrd = 0;
        }
        if ((c != ' ') && (nachalo_flag == 0)) {
            start_first_word = count;
            nachalo_flag = 1;
        }
        if (count % 4 == 0) {
            a = realloc(a, N * ((count/4) + 1));
        }
        if ((c == ' ')) {
            start_word_flag = 0;
            space_flag = 1;
        } else {
            space_flag = 0;
        }
    }    
\
    if ((end_first_word == 0) || (end_first_word == end_last_word)) {
        if (count % 4 == 0) {
            a = realloc(a, N * (count/4 + 1));
        }
        a[count] = '\0';
        return a;
    } 

    if (end_lst_wrd == 0) {
        end_last_word = count;
    }
    count = end_last_word;


    char *first_word = malloc(end_first_word + 1);
    strncpy(first_word, &a[start_first_word], end_first_word - start_first_word + 1);

    int last_word_len = end_last_word - start_last_word;

    char *last_word = malloc(last_word_len + 1);
    strncpy(last_word, &a[start_last_word], last_word_len);

    int ln = count;

    char *otv = malloc(ln + 1);

    int mid_len = ln - end_first_word - last_word_len;

    memcpy(otv, last_word, last_word_len);
    memcpy(&otv[last_word_len], &a[end_first_word], mid_len + 2);
    memcpy(&otv[ln - end_first_word], first_word, end_first_word - start_first_word + 1);
    otv[ln] = '\0';

    strcpy(a, otv);

    free(otv);
    free(last_word);
    free(first_word);
    return a;
}

int main() {
    char *a = NULL;

    a = Input_with_change(a);
    
    if (a) {
        printf("------------------------------------------\n");
        puts(a);
        printf("------------------------------------------\n");
    } else {
        printf("Null\n");
    }
    free(a);
    return 0;
}
