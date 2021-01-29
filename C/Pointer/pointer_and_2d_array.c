#include <stdio.h>

void func(int A[][3])
{
}

int main()
{
    // int B[2][3] = {{2, 4, 6}, {9, 5, 6}};

    // printf("%d\n", B[0][0]);
    // printf("%d\n", *(*B + 1) + 1);

    // int(*p)[3] = B;

    // printf("%d\n", B);
    // printf("%d\n", *B);
    // printf("%d\n", B[0]);

    int C[3][2][2] = {
        {{2, 5}, {7, 9}},
        {{3, 4}, {6, 1}},
        {{0, 8}, {11, 14}}};

    // C = int (*)[2][2]
    //printf("%d %d %d %d\n", C, *C, C[0], &C[0][0]);

    //func(C); //cannot convert ‘int (*)[2][2]’ to ‘int*

    int A[2] = {3, 4};
    //func(A);

    int B[2][3] = {{2, 5}, // B returns int (*)[3]
                   {7, 9}};

    func(B);
}