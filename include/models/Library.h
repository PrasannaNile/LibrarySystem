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
    std::vector<Transaction> transactions;

public:
    Library();
    void add_book(const Book& new_book);
    void issue_book(const std::string& bookId, const std::string& userId);
    void return_book(const std::string& bookId, const std::string& userId);
    void save_books_to_file() const;
    void load_books_from_file();
    void search_book(std::string& searchQuery) const;
    void display_books() const;

    void register_user(const User& new_user);
    void save_user_to_file() const;
    void load_user_from_file();
    void display_users() const;



    bool is_valid_transaction(const std::string& bookId, const std::string& userId);

};