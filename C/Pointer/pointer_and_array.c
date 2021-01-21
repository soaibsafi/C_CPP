#include <stdio.h>

int main()
{
    int A[] = {3, 5, 9, 2, 6};
    printf("A: %p\n", A);           //0x7ffede23b750
    printf("&A: %p\n", &A);         //0x7ffede23b750
    printf("A[0]: %d\n", A[0]);     // 3
    printf("*A: %d\n", *A);         // 3
    printf("*A+1: %d\n", *(A + 1)); // 5
    printf("*A+2: %d\n", *(A + 2)); // 9
    printf("============\n"); // 9

    for (int i = 0; i < 5; i++)
    {
        printf("A: %p\n", A+i);       
        printf("&A: %p\n", &A[i]);     
        printf("A[0]: %d\n", A[i]); 
        printf("*A: %d\n\n", *(A+1));    
        
    }

    int *p = A;
    // A++; // Invalid
    p++; // Valid
}