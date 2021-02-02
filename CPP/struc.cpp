#include<iostream>
#include<cstring>

struct Book{
    char title[50];
    char author[50];
    char subject[50];
    int book_id;
    double price;
};

void print_book(struct Book b)
{
    std::cout << "ID: " << b.book_id << std::endl;
    std::cout << "Title: " << b.title << std::endl;
    std::cout << "Author: " << b.author << std::endl;
    std::cout << "Subject: " << b.subject << std::endl;
    std::cout << "Price: " << b.price << std::endl;
}

int main()
{   
    struct Book book1;
    struct Book book2;

    strcpy(book1.title,  "Make Time");
    strcpy(book1.author, "ABC");
    strcpy(book1.subject, "Productivity");
    book1.book_id = 1001;
    book1.price = 12.5;

    strcpy(book2.title,  "Book2");
    strcpy(book2.author, "XYZ");
    strcpy(book2.subject, "Science");
    book2.book_id = 2001;
    book2.price = 9.5;

    //std::cout << book1.title << std::endl;
    //std::cout << book2.title << std::endl;

    print_book(book1);
    print_book(book2);
}