#include <stdio.h>

void print(char *c)
{
    while (*c != '\0')
    {
        printf("%c", *c);
        c++;
    }
    printf("\n");
}

int main()
{
    char c[20] = "Hello World";
    print(c);
}