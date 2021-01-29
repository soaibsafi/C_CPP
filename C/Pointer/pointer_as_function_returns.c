#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;
    printf("Enter the size of the array: ");
    scanf("%d", &n);

    int *A = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        A[i] = i;
    }
    int *B = (int *)realloc(A, 2 * n * sizeof(int));

    for (int i = 0; i < 2 * n; i++)
    {
        printf("%d ", B[i]);
    }
}