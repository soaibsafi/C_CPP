#include<iostream>

int main()
{
    int guesses[10] = {22, 44,63,24,23, 75,12};
    int size = sizeof(guesses) / sizeof(int);
    std::cout << size <<std::endl;
    for(int i = 0; i < size; i++){
        std::cout << guesses[i] <<"\t";
    }

    return 0;
}