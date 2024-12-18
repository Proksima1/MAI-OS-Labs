#include "stdio.h"
#include "dlfcn.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

typedef int (*gcd_func)(int, int);

typedef double (*pi_func)(int);

void checkDlError(char *error) {
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }
}


int main() {
    void *handle;
    gcd_func GCD_func;
    pi_func PI_func;
    char *error;

    handle = dlopen("./liblib_funcs.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return EXIT_FAILURE;
    }

    GCD_func = dlsym(handle, "GCD_Euclid");
    checkDlError(error);

    PI_func = dlsym(handle, "PiLeibniz");
    checkDlError(error);


    bool isFirstImp = true;
    int variant;
    while (true) {
        printf("Enter variant number(1 - GCD, 2 - PI) and arguments or 0 if you want to swap implementation: ");
        scanf("%d", &variant);
        if (variant == 1) {
            int first, second;
            scanf("%d %d", &first, &second);
            int first_gcd = GCD_func(first, second);
            printf("Answer - %d\n", first_gcd);
        } else if (variant == 2) {
            int k;
            scanf("%d", &k);
            double PI = PI_func(k);
            printf("Answer - %.10f\n", PI);
        } else if (variant == 0) {
            if (isFirstImp) {
                GCD_func = dlsym(handle, "GCD_Naive");
                checkDlError(error);
                PI_func = dlsym(handle, "PiWallis");
                checkDlError(error);
                printf("Swapped to GCD naive and Pi Wallis\n");
                isFirstImp = false;
            } else {
                GCD_func = dlsym(handle, "GCD_Euclid");
                checkDlError(error);
                PI_func = dlsym(handle, "PiLeibniz");
                checkDlError(error);
                printf("Swapped to GCD Euclid and Pi Leibniz\n");
                isFirstImp = true;
            }
        }
    }
    dlclose(handle);
}