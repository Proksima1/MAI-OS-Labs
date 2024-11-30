#include "pi.h"
#include "gcd.h"
#include "stdio.h"

int main() {
    int variant;
    printf("Enter variant number(1 - GCD, 2 - PI) and arguments: ");
    scanf("%d", &variant);
    if (variant == 1) {
        int first, second;
        scanf("%d %d", &first, &second);
        int first_gcd = GCD_Euclid(first, second);
        int second_gcd = GCD_Naive(first, second);
        printf("Euclid - %d\n", first_gcd);
        printf("Naive - %d", second_gcd);
    } else if (variant == 2) {
        int k;
        scanf("%d", &k);
        double PI = PiWallis(k);
        double PI2 = PiLeibniz(k);
        printf("Wallis - %.10f\n", PI);
        printf("Leibniz - %.10f", PI2);
    }
}