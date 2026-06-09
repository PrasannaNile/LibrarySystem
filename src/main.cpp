#include "models/Library.h"
#include <iostream>

int main() {
    Library lib;

    // 1. Setup Data
    // lib.add_book(Book("B03", "The DSA master class", "abc1", 500.50));
    // lib.add_book(Book("B04", "OOPs paradiagm", "abc2", 899.00));

    lib.display_books();

    return 0;
}