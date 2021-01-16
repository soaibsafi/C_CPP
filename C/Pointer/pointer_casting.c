#include<stdio.h>

int main(){
    int a = 1025; // bin: 00000000 00000000 00000100 00000001
    int *ip = &a; 

    printf("Address: %p, Value: %d\n", ip, *ip);

    char b;
    char *cp;

    cp = (char*)ip;
    printf("Address: %p, Value: %d\n", cp, *cp); //00000001 = 1
    printf("Address: %p, Value: %d\n", cp+1, *(cp+1));  // 00000100 = 4
    printf("Address: %p, Value: %d\n", cp+2, *(cp+2));  // 00000000 = 0

    // Void pointer
    void* vp;
    vp = ip;
    printf("Address: %p\n", vp); 

}