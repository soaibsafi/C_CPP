#include<stdio.h>

void change_value(int* x){*x = 7;}

int main(){
    int a=5;
    int *p = &a;
    // printf("%p\n", p);  //%p is used to printing the address of a pointer 
    // printf("%d\n", *p);  
    // printf("%p\n", &a);  

    // printf("%d\n", a);
    // *p = 8;
    // printf("%d\n", a);

    printf("Address of p: %p\n", p);
    printf("Value of p: %d\n", *p);
    printf("Value of a: %d\n", a);
    int b = 10;
    *p = b;
    printf("Address of p: %p\n", p);
    printf("Value of p: %d\n", *p);
    printf("Value of a: %d\n", a);
}