#include <stdio.h>

int main()
{
    int B[2][3] = {{2, 4, 6}, {9, 5, 6}};

    printf("%d\n", B[0][0]);
    printf("%d\n", *(*B + 1) + 1);
}