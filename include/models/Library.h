# pragma once

#include <vector>
#include <string>
#include <unordered_map>


#include "models/Book.h"
#include "models/User.h"
#include "models/Transaction.h"



class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;
    std::vector<Transaction> transactions;
    std::unordered_map<std::string, std::string> active_loans;
    

public:
    Library();
    void add_book(const Book& new_book);
    void issue_book(const std::string& bookId, User& user);
    void return_book(const std::string& bookId, User& userId);
    void save_books_to_file() const;
    void load_books_from_file();
    void search_book(std::string& searchQuery) const;
    Book* search_book_by_id(const std::string& bookId);
    void display_books() const;
    void display_active_loans() const;

    void register_user(const User& new_user);
    void save_user_to_file() const;
    void load_user_from_file();
    void search_user(const std::string& userId) const;
    void display_users() const;



    bool is_valid_transaction(const std::string& bookId, const std::string& userId);
    void save_transactions_to_file() const;
    void load_transactions_from_file();
};