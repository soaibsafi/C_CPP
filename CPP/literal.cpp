#include<iostream>

int main()
{
    int a = 212;
    int b = 0245;
    int c = 0xFEE;
    int d = 0xFEEL;
    
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << sizeof(c) << std::endl;
    std::cout << sizeof(d) << std::endl;
    return 0;
}