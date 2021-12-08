#include <stdio.h>

// Sagt Hallo und gibt Parameter aus.
int main(int c, char** v) {
    printf("Hallo von test_print.\n");
    printf("Parameter:\n");
    for (int i = 0; i < c; i++) {
        printf("param %d: %s\n", i, v[i]);
    }
}