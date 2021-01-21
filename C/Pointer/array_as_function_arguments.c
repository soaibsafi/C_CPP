#include <stdio.h>

int sum_of_elements(int *A, int size)
{
    int i, sum = 0;

    for (i = 0; i < size; i++)
    {
        sum += A[i];
    }
    return sum;
}

void double_elements(int *A, int size)
{
    for (int i = 0; i < size; i++)
    {
        A[i] = A[i] * 2;
    }
}

int main()
{
    int A[] = {1, 2, 3, 4, 5};

    int size = sizeof(A) / sizeof(A[0]);
    printf("Size: %d\n", size);

    int total = sum_of_elements(A, size);
    printf("Sum: %d\n", total);

    double_elements(A, size);

    for(int i = 0; i<size; i++)
    {
        printf("A[%d]: %d\n", i, A[i]);
    }
}