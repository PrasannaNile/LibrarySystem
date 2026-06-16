#include "models/User.h"

#include <iostream>

int User::countId = 1001;

// constructor with auto generate userId
User::User(const std::string& name, const std::string& email, const UserRole& role) {
    this->userId = "U" + std::to_string(countId);
    id_incrementor();
    this->name = name;
    this->email = email;
    this->role = role;
}

// constructor with known userId
User::User(const std::string& userId, const std::string& name, const std::string& email, const UserRole& role) {
    this->userId = userId;
    this->name = name;
    this->email = email;
    this->role = role;

    int numericId = std::stoi(userId.substr(1));
    if(numericId >= countId) {
        countId += 1;
    }
}

User::User(const User& other) {
    this->userId = other.userId;
    this->name = other.name;
    this->email = other.email;
    this->role = other.role;
}


std::string User::get_userId() const { return userId; }
std::string User::get_name() const { return name; }
std::string User::get_email() const { return email; }
UserRole User::get_role() const { return role; }
void User::id_incrementor() { countId++; }

void User::borrow_book(Book* bookPtr) {
    borrowedBooks.push_back(bookPtr);
}


void User::return_book(Book* bookPtr) {
    borrowedBooks.erase(
        std::remove(borrowedBooks.begin(), borrowedBooks.end(), bookPtr), 
        borrowedBooks.end()
    );
}


void User::display_borrowed_books() const {
    if(borrowedBooks.size() == 0) std::cout << "No books borrowed!\n";
    else {

        for(const auto book: borrowedBooks) {
            std::cout << book->get_bookId() << " | " << book->get_title() << " | " << book->get_author() << "| "
                      << book->get_price() << "\n";
        }

    }
}