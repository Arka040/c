#include <stdio.h>
#include <stdlib.h>

int count(char *f, int length, int n) {
    int c = 0;
    int mc = 0;
    for (int i = 0; i < length; i++) {
        unsigned mask = 0x80;
        int b;
        for (int j = 0; j < 8; j++) {
            b = ((f[i] & mask) >> (7 - j));
            mask >>= 1;
            if (b == 1) {
                mc++;
            } else {
                if (mc >= n) {
                    c++;
                }
                mc = 0;
            }
        }
    }
    if (mc >= n) {
        c++;
    }
    return c;
}

int max_len(char *f, int length, int n) {
    int max = 0;
    int mc = 0;
    for (int i = 0; i < length; i++) {
        unsigned mask = 0x80;
        int b;
        for (int j = 0; j < 8; j++) {
            b = ((f[i] & mask) >> (7 - j));
            mask >>= 1;
            if (b == 1) {
                mc++;
            } else {
                if (mc > max) {
                    max = mc;
                }
                mc = 0;
            }
        }
    }
    if (mc > max) {
        max = mc;
    }
    return max;
}

int main() {
    int length = 12;
    char string[] = "Hello World!";
    int n;
    char *f = string;

    printf("Enter length byte: ");
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

    printf("count >= n: ");
    printf("%d\n", count(f, length, n));

    printf("max_count: ");
    printf("%d\n", max_len(f, length, n));

    return 0;
}
