#include <stdio.h>

int Bubble_sort(int l, int A[l]) {
    int c;

    for (int i = 0; i < l - 1; i ++) {
        for (int j = 0; j < l - 1 - i; j++) {
            if (A[j] > A[j + 1]) {
                c = A[j + 1];
                A[j + 1] = A[j];
                A[j] = c;
            }
        }
    }
    return A[l];
}

int main() {
    int c;
    int l;

    printf("Enter array length: ");
    scanf("%d", &l);

    int A[l];
    
    for (int i = 0; i != l; i++) {
        printf("Enter elemenet: ");
        scanf("%d", &c);
        A[i] = c;        
    }

    Bubble_sort(l, A);

    for (int i = 0; i != l; i++) {
        printf("%d ", A[i]); 
    }
    
    printf("\n");

    return 0;
}
