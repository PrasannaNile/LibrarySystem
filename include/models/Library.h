# pragma once

#include <vector>
#include <string>
#include <fstream>

#include "models/Book.h"
#include "models/User.h"
#include "models/Transaction.h"



class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;
    std::vector<Transaction> transactions;

public:
    Library();
    void add_book(const Book& new_book);
    void register_user(const User& new_user);
    void issue_book(const std::string& bookId, const std::string& userId);
    void return_book(const std::string& bookId, const std::string& userId);
    bool is_valid_transaction(const std::string& bookId, const std::string& userId);
    void display_books() const;
    void save_books_to_file() const;
    void load_books_from_file();

};