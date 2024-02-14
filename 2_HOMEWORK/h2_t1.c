#include <stdio.h>

void spiral(int n, int m, int arr[n][m]) {
    int right_border = 0;
    int left_border = 0;
    int up_border = 0;
    int down_border = 0;

    int i = 0;
    int j = 0;
    int num = 1;

    while (num <= n*m) {
        arr[i][j] = num;
        if (i == up_border && j < m - right_border - 1){
            j++;
        }
        else if (j == m - right_border - 1 && i < n - down_border - 1) {
            i++;
        }
        else if (i == n - down_border - 1 && j > left_border) {
            j--;
        }
        else {
            i--;
        }
        
        if (i == up_border + 1 && j == right_border && right_border != m - right_border - 1) {
            up_border++;
            right_border++;
            down_border++;
            left_border++;
        }
        num++;
    }

}


int main() {
    int n;
    int m;

    printf("Input n: ");
    scanf("%d", &n);

    printf("Input m: ");
    scanf("%d", &m);

    int arr[n][m];

    spiral(n, m, arr);
    
    for (int k = 0; k < n; k++) {
        for (int h = 0; h < m; h++) {
            printf(" %-3d ", arr[k][h]);
        }
        printf("\n");
    }

    return 0;
}