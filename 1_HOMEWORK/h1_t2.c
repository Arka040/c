#include <stdio.h>

void print_binary(int x, int l, int A[l]) {
    unsigned k = 2147483648;
    int b;
    printf(" %d ", x);
    for (int j = 0; j < l; j++) {
        b = ((x & k) >> (l - 1- j));
        printf("%d", b);
        A[j] = b;
        k >>= 1;
    }
    printf("\n");   
}

int shift(int x, int m, int n, int l, int A[l]) {
    int c;
    n %= l;     
    if (m == 1) {
        for (int i = 0; i < n; i++) {
            c = A[l - 1];
            for (int j = l - 1; j > 0; j--) {
                A[j] = A[j - 1];
            }
            A[0] = c;
        }
    } else {
        for (int i = 0; i < n; i++) {
            c = A[0];
            for (int j = 0; j < l; j++) {
                A[j] = A[j + 1];
            }
            A[l - 1] = c;
        }
    }
    x = 0;

    for (int i = 0; i < l; i++) {
        x = x * 2 + A[i];
    }

    return x;
}

int main () {
    int x;
    int n;
    int move;
    int len = sizeof(int) * 8;
    int A[len];
    printf("Enter a number: ");
    scanf("%d", &x);
    
    print_binary(x, len, A);

    printf("Input shift num: ");
    scanf("%d", &n);

    printf("Input -1 to shift to left and 1 to shift to right\n");
    scanf("%d", &move);

    x = shift(x, move, n, len, A);
    print_binary(x, len, A);

    return 0;
}

