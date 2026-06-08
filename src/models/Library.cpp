#include "models/Library.h"

#include <iostream>


void Library::add_book(const Book& new_book) {
    books.push_back(new_book);
}

void Library::register_user(const User& new_user) {
    users.push_back(new_user);
}

void Library::display_book() const {
    for (const auto& book : books) {
        std::cout << "ID: " << book.get_bookId() << " | Title: " << book.get_title() << "\n";
    }
}