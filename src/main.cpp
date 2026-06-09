#include "models/Library.h"
#include <iostream>

int main() {
    Library lib;

    // 1. Setup Data
    Book b1("The oops master class", "abc3", 499.50);
    lib.add_book(b1);


    // lib.display_books();

    return 0;
}