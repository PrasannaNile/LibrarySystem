# pragma once

#include <vector>
#include <string>

#include "models/Book.h"
#include "models/User.h"
#include "models/Transaction.h"


class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;
    const std::vector<Transaction> transactions;

public:
    Library() = default;
    void add_book(const Book& new_book);
    void register_user(const User& new_user);
    void issue_book(const std::string& bookId, const std::string& userId);
    void return_book(const std::string& bookId, const std::string& userId);
    bool is_valid_transaction(const std::string& bookId, const std::string& userId);
    void display_books() const;

};