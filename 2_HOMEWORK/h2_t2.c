#include <stdio.h>

void print_binary(int n, int arr[n]) {
    for (int i = 0; i < n; i++) {
        unsigned k = 2147483648;
        int b;
        printf(" %d ", arr[i]);
        for (int j = 0; j < sizeof(int) * 8; j++) {
            b = ((arr[i] & k) >> (sizeof(int) * 8 - 1- j));
            printf("%d", b);
            k >>= 1;
        }
        printf("\n");
    }
}

int ones_counter(int x) {
    int count = 0;
    int mask = 1;
    for (int i = 0; i < sizeof(int) * 8; i++) {
        if ((mask & x) != 0) {
            count++;
        }
        mask <<= 1;
    }
    return count;
}

int sort(int n, int arr[n]) {
    int c;
    int a;
    int b;
    for (int i = 0; i < n - 1; i ++) {
        for (int j = 0; j < n - 1 - i; j++) {
            a = ones_counter(arr[j]);
            b = ones_counter(arr[j + 1]);
            if (a > b) {
                c = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = c;
            }
            else if (a == b) {
                if (arr[j] <= arr[j + 1]) {
                    c = arr[j + 1];
                    arr[j + 1] = arr[j];
                    arr[j] = c;
                }
                
            }
        }
    }
    return arr[n];
}

int main () {

    int n;
    printf("Input length of array: ");
    scanf("%d", &n);

    int arr[n];
    for (int i = 0; i < n; i++) {
        printf("Input num for arr: ");
        scanf("%d", &arr[i]);
    }

    print_binary(n, arr);

    arr[n] = sort(n ,arr);

    for (int i = 0; i < n; i++) {
        printf(" %d ", arr[i]);
    }

    printf("\n");   

    return 0;
}