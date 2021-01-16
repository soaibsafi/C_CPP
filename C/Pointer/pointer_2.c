#include<stdio.h>

void change_value(int** x, int* y) { *x = y; }

int main(){
    int a = 5; 
    int b = 7;
    int* p = &a;
    printf("%d\n", *p);
    change_value(&p, &b);
    printf("%d", *p);

}