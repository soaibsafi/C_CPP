#include<stdio.h>

int main()
{
    int x = 5;
    int* p = &x;

    int** q = &p;

    int*** r = &q;


    printf("Address  of x = %p\n", &x);  // 0x1418c
    printf("Value  of x = %d\n", x);     //5

    printf("Address of p = %p\n", p);   // 0x1418c - Pointer to x
    printf("Value of *p = %d\n", *p);   // 5 - Value of the pointer 

    printf("Address of q = %p\n", q);   // 0x14190 - Pointer to p
    printf("Value of *q = %p\n", *q);   // 0x1418c - Address of p (*q = &p;) (one Dereferenceing) 
    printf("Value of **q = %d\n", *(*q));   //5 - Value of the pointer  **q = &p;

    printf("Address of r = %p\n", r);   // 0x14198 - Pointer to q
    printf("Value of *r = %p\n", *r);   // 0x14190 - Address of q (*r = &q;) (one Dereferenceing) 
    printf("Value of **r = %p\n", *(*r));   // 0x1418c - Address of p (**r = &q;) 
    printf("Value of ***r = %d\n", **(*r));   //5 - Value of the pointer  ***r = &q;


}