#include<stdio.h>

void Increment(int *a)
{
    *a = *a+1;
}

int main()
{
    int a = 5;

    Increment(&a);
    printf("Value of a: %d\n", a);
}