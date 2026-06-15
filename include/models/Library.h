# pragma once

#include <vector>
#include <string>
#include <unordered_map>


#include "models/Book.h"
#include "models/User.h"
#include "models/Transaction.h"
#include "database/DBManager.h"



class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;
    std::vector<Transaction> transactions;
    std::unordered_map<std::string, std::string> active_loans;
    std::unordered_map<std::string, bool> registered_emails;

    DBManager* db;
    

public:
    Library(DBManager* db);
    void add_book(const Book& new_book);
    // void issue_book(const std::string& bookId, User& user); // deprecated
    void issue_book(const std::string& bookId, User& user);
    void return_book(const std::string& bookId, User& userId);
    void save_books_to_file() const; // deprecated
    void load_books_from_file(); // deprecated
    void load_books_from_database();
    void search_book(std::string& searchQuery) const;
    Book* search_book_by_id(const std::string& bookId);
    void display_books() const;
    void display_active_loans() const;
    void update_book_status(const std::string& bookId, BookStatus status);
    std::string generate_next_user_id();
    int get_borrowed_books_count(const std::string& userId);



    User* register_user(const User& new_user);
    bool is_email_registered(const std::string& email) const;
    void save_user_to_file() const; // deprecated
    void load_user_from_file();
    void load_users_from_database();
    void search_user(const std::string& userId) const;
    User* search_user_by_id(const std::string& userId);
    void display_users() const;
    std::string generate_next_book_id();



    bool is_valid_transaction(const std::string& bookId, const std::string& userId);
    void save_transactions_to_file() const; // deprecated
    void save_transaction_to_db(const Transaction& new_tran);
    void load_transactions_from_file(); // deprecated
    void load_transactions_from_database();
    std::string generate_next_transaction_id();
};