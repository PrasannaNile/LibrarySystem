#include "models/Library.h"

#include <iostream>


void Library::add_book(const Book& new_book) { books.push_back(new_book); }


void Library::register_user(const User& new_user) { users.push_back(new_user); }


void Library::issue_book(const std::string& bookId, const std::string& userId) {
    for(auto& book: books) {
        if(book.get_bookId() == bookId) {
            BookStatus curr_status = book.get_status();
            if(curr_status == BookStatus::AVAILABLE) {

                std::string newTxnId = "TXN-" + std::to_string(transactions.size() + 1);
                transactions.push_back(Transaction(newTxnId, bookId, userId, TransactionType::ISSUED));

                book.set_status(BookStatus::LOANED);
                std::cout << "Success: Book " << bookId << " successfully issued to user " << userId << ".\n";
                return;
            } else {
                std::cout << "Error: Book " << bookId << " is currently not available (Status: LOANED/RESERVED/LOST).\n";
                return;
            }
        }
    }

    std::cout << "Error: Book with ID " << bookId << " does not exist in the library catalog.\n";
}


void Library::return_book(const std::string& bookId, const std::string& userId) {
    for(auto& book: books) {
        if(book.get_bookId() == bookId) {
            BookStatus curr_status = book.get_status();

            if(curr_status != BookStatus::LOANED) {
                std::cout << "Error: User " << userId << " not current holds the book " << bookId << ".\n";
                return;

            } else {
                if(is_valid_transaction(bookId, userId)) {

                    std::string newTxnId = "TXN-" + std::to_string(transactions.size() + 1);
                    transactions.push_back(Transaction(newTxnId, bookId, userId, TransactionType::RETURNED));
                    
                    book.set_status(BookStatus::AVAILABLE);
                    std::cout << "Success: Book " << bookId << " successfully returned by user " << userId << ".\n";
                    return;
                }
            }
        }
    }

    std::cout << "Error: Book " << bookId << " not found in library." << "\n";
}



bool Library::is_valid_transaction(const std::string& bookId, const std::string& userId) {
    for(auto it = transactions.rbegin(); it != transactions.rend(); ++it) {
        if(it->get_bookId() == bookId && it->get_userId() == userId && it->get_type() == TransactionType::ISSUED) {
            return true;
        }
    }
    return false;
}



void Library::display_books() const {
    for (const auto& book : books) {
        std::cout << "ID: " << book.get_bookId() << " | Title: " << book.get_title() << "\n";
    }
}