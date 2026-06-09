#include "models/Library.h"
#include <iostream>

int main() {
    Library lib;

    // 1. Setup Data
    // Book b1("The oops master class", "abc3", 499.50);
    // lib.add_book(b1);

    // User u1("abc", "abc@gmail.com", UserRole::STUDENT);
    // lib.register_user(u1);


    lib.display_books();
    lib.display_users();


    return 0;
}