#include <stdio.h>
#define BINPRINT(x) {                                                           \
    typeof(x) num = x;                                                          \
    int size = sizeof(num) * 8;                                                 \
    for (int i = size - 1; i >= 0; i--) {                                       \
        printf("%d", (int)((x >> i) & 1));                                      \
    }                                                                           \
    printf("\n");                                                               \
}

int main () {
    char c=-3;
    if(c) {
        BINPRINT(c);
    } else {
        printf("Zero!\n");
    }
    BINPRINT((short)7);
    long x=15;
    if(x) {
    BINPRINT(x);
    } else {
        printf("Zero!\n");
    }
    BINPRINT((long long)x);
    return 0;
}