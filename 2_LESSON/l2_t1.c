#include <stdio.h>
#include <stdlib.h>

char find_byte_right_left(char *f, int length, int n) {
    int byte = n / 8;
    int bit = n % 8;
    int k = 1;
    int otv;

    k <<= bit;
    otv = (f[byte] & k) >> bit;

    return otv;
    
}

char find_byte_left_right(char *f, int length, int n) {
    int byte = n / 8;
    int bit = 7 - (n % 8);
    int k = 1;
    int otv;

    k <<= bit;
    otv = (f[byte] & k) >> bit;

    return otv;
}

int main() {
    int length = 12;
    char string[] = "Hello World!";
    int n;
    char *f = string;

    printf("Enter byte: ");
    scanf("%d", &n);

    for (int i = 0; i < length; i++) {
        printf(" %c ", *(f+i));
        unsigned k = 0x80;
        int b;
        for (int j = 0; j < 8; j++) {
            b = ((f[i] & k) >> (7 - j));
            printf("%d", b);
            k >>= 1;
        }
        printf("\n");
    }
    printf("right_left byte: ");
    printf("%d\n", find_char_right_left(f, length, n));

    printf("left_right byte");
    printf("%d\n", find_char_left_right(f, length, n));

    return 0;
}