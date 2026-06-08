# pragma once

#include <vector>
#include <string>

#include "models/Book.h"
#include "models/User.h"


class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;
public:
    Library() = default;
    void add_book(const Book& new_book);
    void register_user(const User& new_user);
    void display_book() const;

};