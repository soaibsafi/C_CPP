#include<stdio.h>

void change_value(int *x) { *x = 7; }
void apple_change(void (*func)(int*), int* y){ (*func)(y);}

int main(){
    void(*ptr)(int*) = &change_value;
    int a = 3;
    (*ptr)(&a);

    void(*foo)(int*);
    foo = ptr;

    apple_change(ptr, &a);
    int(*func)(int*) = ptr;
}