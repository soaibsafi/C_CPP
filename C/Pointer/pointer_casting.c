#include<stdio.h>

int main(){
    int a; float b;
    int* ip = &a;
    float* fp = &b;

    *fp = 4.3;
    *ip = 7.3;

    //printf("%d\n%d\n", fp, ip);
    //printf("%f\n%f\n", *fp, *ip);

    //ip = fp;

    int x = *ip;
    printf("%d\n", x);

    float y = (float)*ip;
    printf("%d\n", y);

    float z = *(float*)ip;
    printf("%d\n", z);
}