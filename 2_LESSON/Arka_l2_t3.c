#include <stdio.h>


int main () {
    int n1, m1, n2, m2;
    printf("Input n1: ");
    scanf("%d", &n1);
    printf("Input m1: ");
    scanf("%d", &m1);
    printf("Input n2: ");
    scanf("%d", &n2);
    printf("Input m2: ");
    scanf("%d", &m2);

    int arr1[n1][m1];
    int arr2[n2][m2];
    int arr[n1][m1];

    for (int i = 0; i < m1; i++) {
        for (int j = 0; j < n1; j++) {
            printf("Input number for first arr ");
            scanf("%d", &arr1[i][j]);
        }
    }

    for (int i = 0; i < m2; i++) {
        for (int j = 0; j < n2; j++) {
            printf("Input number for second arr ");
            scanf("%d", &arr2[i][j]);
        }
    }

    int x, y;
    printf("Input x: ");
    scanf("%d", &x);
    printf("Input y: ");
    scanf("%d", &y);

    for (int i = 0; i < m1; i++) {
        for (int j = 0; j < n1; j++) {
            arr[i][j] = arr1[i][j];
            if (i - y >= 0 && j - x >= 0 && i - y < m2 && j - x < n2) {
                if (__INT_MAX__ - arr2[i - y][j - x] < arr[i][j]) {
                    arr[i][j] = __INT_MAX__;
                } else {
                    arr[i][j] += arr2[i-y][j - x];
                }

            }
        }
    }

    for (int i = 0; i < m1; i++) {
        for (int j = 0; j < n1; j++) {
            printf(" %d ", arr[i][j]);
        }
        printf("\n");
    }
    return 0;
}
