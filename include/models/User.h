# pragma once

#include "models\Book.h"

#include <string>
#include <vector>
#include<algorithm>

class Book;

enum class UserRole {ADMIN = 1, STUDENT = 2};


class User {
private:
    std::string userId;
    std::string name;
    std::string email;
    UserRole role;
    std::vector<Book* > borrowedBooks;
    static int countId;

public:
    User() = default;
    User(const std::string& _name, const std::string& _email, const UserRole& _role);
    User(const std::string& _userId, const std::string& _name, const std::string& _email, const UserRole& _role);
    User(const User& other);

    std::string get_userId() const;
    std::string get_name() const;
    std::string get_email() const;
    UserRole get_role() const;
    static void id_incrementor();

    void borrow_book(Book* bookPtr);
    void return_book(Book* bookPtr);
    void display_borrowed_books() const;

};