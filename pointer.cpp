#include <iostream>

int main()
{
    int v1 = 20;
    char v2[20];

    int *ip;
    //std::cout << &v1 << std::endl << & v2 << std::endl;
    ip = &v1;

    //std::cout << v1 << std::endl;
    //std::cout << ip << std::endl;
    //std::cout << *ip <<std::endl;

    int *ptr = NULL;
    //std::cout << ptr << std::endl;

    int arr[] = {44, 23, 89};
    ptr = arr;

    for (int i = 0; i < 3; i++)
    {
        //std::cout << ptr <<"\t" << *ptr << std::endl;

        //ptr++;
    }

    // Pointer comparison
    while (ptr <= &arr[2])
    {
        std::cout << ptr << "\t" << *ptr << std::endl;
        ptr++;
    }

    return 0;
}