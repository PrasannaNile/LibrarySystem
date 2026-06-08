#include "models/Library.h"

#include <iostream>


void Library::add_book(const Book& new_book) {
    books.push_back(new_book);
}

void Library::register_user(const User& new_user) {
    users.push_back(new_user);
}

void Library::issue_book(const std::string bookId, const std::string user_id) {
    for(auto& book: books) {
        if(book.get_bookId() == bookId) {
            BookStatus curr_status = book.get_status();
            if(curr_status == BookStatus::AVAILABLE) {
                book.set_status(BookStatus::LOANED);

                std::cout << "Success: Book " << bookId << " successfully issued to user " << user_id << ".\n";
                return;
            } else {
                std::cout << "Error: Book " << bookId << " is currently not available (Status: LOANED/RESERVED/LOST).\n";
                return;
            }
        }
    }

    std::cout << "Error: Book with ID " << bookId << " does not exist in the library catalog.\n";
}

void Library::return_book(const std::string bookId, const std::string user_id) {
    for(auto& book: books) {
        
    }
}


void Library::display_book() const {
    for (const auto& book : books) {
        std::cout << "ID: " << book.get_bookId() << " | Title: " << book.get_title() << "\n";
    }
}