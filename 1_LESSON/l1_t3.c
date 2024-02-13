#include <stdio.h>

int f(int l, int A[l], int n) {
    int c;
    n = n%l;
    for (int j = 0; j < n; j ++) {
        c = A[l-1];
        for (int i = l-1; i > 0; i--) {
            A[i] = A[i-1];
        }
        A[0] = c;
    }
    return A[l];
}

int main() {
    int c;
    int l;
    int n;

    printf("Enter array length: ");
    scanf("%d", &l);

    int A[l];
    
    for (int i = 0; i != l; i++) {
        printf("Enter elemenet: ");
        scanf("%d", &c);
        A[i] = c;        
    }

    printf("Enter shift value: ");
    scanf("%d", &n);

    f(l, A, n);

    for (int i = 0; i != l; i++) {
        printf("%d ", A[i]); 
    }
    
    printf("\n");
    return 0;
}