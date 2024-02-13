#include <stdio.h>

double my_abs(double a, double b) {
    double m;
    m = a - b;
    return m * ((m >= 0) - (m < 0));
}

int main() {
    double x;
    double n;
    double x0;
    double xi;
    printf("Enter epsilon: ");
    scanf("%lf", &n);

    while ((scanf("%lf", &x)) != EOF) {
        x0 = 1;
        xi = (x0 + x/x0)/2;
        while (my_abs(x0, xi) >= n) {
            x0 = xi;
            xi = (x0 + x/x0)/2;
        }   
        printf("square root of %lf is ", x);
        printf("%f\n", xi);
        printf("square root of %lf is ", x);
        printf("%g\n", xi);
        printf("square root of %lf is ", x);
        printf("%e\n", xi);
        printf("square root of %lf is ", x);
        printf("%.10g\n", xi);
        
        
    }

    return 0;
}
