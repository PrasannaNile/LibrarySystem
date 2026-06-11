#include "models/Library.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>


Library::Library() {
    load_books_from_file();
    load_user_from_file();
}


void Library::add_book(const Book& new_book) { 
    books.push_back(new_book); 
    save_books_to_file();
}


void Library::issue_book(const std::string& bookId, User& user) {
    for(auto& book: books) {
        if(book.get_bookId() == bookId) {
            BookStatus curr_status = book.get_status();
            if(curr_status == BookStatus::AVAILABLE) {

                std::string newTxnId = "TXN-" + std::to_string(transactions.size() + 1);
                transactions.push_back(Transaction(newTxnId, bookId, user.get_userId(), TransactionType::ISSUED));

                book.set_status(BookStatus::LOANED);
                user.borrow_book(&book);

                std::cout << "Success: Book " << bookId << " successfully issued to user " << user.get_userId() << ".\n";
                save_books_to_file();
                return;
            } else {
                std::cout << "Error: Book " << bookId << " is currently not available (Status: LOANED/RESERVED/LOST).\n";
                return;
            }
        }
    }

    std::cout << "Error: Book with ID " << bookId << " does not exist in the library catalog.\n";
}


void Library::return_book(const std::string& bookId, User& user) {
    for(auto& book: books) {
        if(book.get_bookId() == bookId) {
            BookStatus curr_status = book.get_status();

            if(curr_status != BookStatus::LOANED) {
                std::cout << "Error: User " << user.get_userId() << " not currently holds the book " << bookId << ".\n";
                return;

            } else {
                if(is_valid_transaction(bookId, user.get_userId())) {

                    std::string newTxnId = "TXN-" + std::to_string(transactions.size() + 1);
                    transactions.push_back(Transaction(newTxnId, bookId, user.get_userId(), TransactionType::RETURNED));
                    
                    book.set_status(BookStatus::AVAILABLE);
                    user.return_book(&book);

                    std::cout << "Success: Book " << bookId << " successfully returned by user " << user.get_userId() << ".\n";
                    save_books_to_file();
                    return;
                }
            }
        }
    }

    std::cout << "Error: Book " << bookId << " not found in library." << "\n";
}


void Library::save_books_to_file() const {
    std::ofstream outFile ("books.txt");
    if(!outFile) {
        std::cerr << "Error: Denied permission or file not exist..\n";
        return;
    }

    for(const auto& book: books) {
        outFile << book.get_bookId() << "|" << book.get_title() << "|" << book.get_author() << "|"
                << book.get_price() << "|" << static_cast<int>(book.get_status()) << "\n";
    }

    outFile.close();
}

void Library::load_books_from_file() {
    std::ifstream inFile("books.txt");
    if(!inFile) {
        std::cerr << "Error: Denied permission or file not exist..\n";
        return;
    }

    std::string line = "";
    while(std::getline(inFile, line)) {
        if (line.empty()) {
            continue; 
        };

        std::stringstream ss(line);

        std::string bookId;
        std::string title;
        std::string author;
        std::string priceStr;
        std::string statusStr;
        

        std::getline(ss, bookId, '|');
        std::getline(ss, title, '|');
        std::getline(ss, author, '|');
        std::getline(ss, priceStr, '|');
        std::getline(ss, statusStr, '|');

        float price = std::stof(priceStr);
        BookStatus status = static_cast<BookStatus> (std::stoi(statusStr));
        
        Book loadedBook(bookId, title, author, price, status);

        books.push_back(loadedBook);

    } 
}

void Library::search_book(std::string& searchQuery) const {

    std::transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    bool book_found = false;

    for(const auto& book: books) {
        std::string title = book.get_title();
        std::string author = book.get_author();

        std::transform(title.begin(), title.end(), title.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        std::transform(author.begin(), author.end(), author.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        if(title.find(searchQuery) != std::string::npos || author.find(searchQuery) != std::string::npos) {
            book_found = true;
            std::cout << book.get_bookId() << "| " << book.get_title() << "| "
                      << book.get_author() << "| " << book.get_price() << "| " 
                      << static_cast<int> (book.get_status()) << "\n";
        }
    }

    if(book_found == false) std::cout << "No Result!" << "\n";
}

Book* Library::search_book_by_id(const std::string& bookId) {
    for(auto& book: books) {
        if(book.get_bookId() == bookId) return &book;
    }

    return nullptr;
}

void Library::display_books() const {
    for (const auto& book : books) {
        std::cout << "ID: " << book.get_bookId() << " | Title: " << book.get_title() << "\n";
    }
}






void Library::register_user(const User& new_user) { 
    users.push_back(new_user); 
    save_user_to_file();
}


void Library::save_user_to_file() const {
    std::ofstream outFile("users.txt");
    if(!outFile) {
        std::cerr << "Error: Denied permission or file not exist..\n";
        return;
    }

    for(const auto& user: users) {
        outFile << user.get_userId() << "|" << user.get_name() << "|" << user.get_email() << "|" 
                << static_cast<int>(user.get_role()) << "\n";
    } 

    outFile.close();
}


void Library::load_user_from_file() {
    std::ifstream inFile("users.txt");
    if(!inFile) {
        std::cerr << "Error: Denied permission or file not exist..\n";
        return;
    }

    std::string line{};
    while(std::getline(inFile, line)) {
        if(line.empty()) continue;

        std::stringstream ss(line);

        std::string userId;
        std::string name;
        std::string email;
        std::string roleStr;

        std::getline(ss, userId, '|');
        std::getline(ss, name, '|');
        std::getline(ss, email, '|');
        std::getline(ss, roleStr, '|');

        UserRole role = static_cast<UserRole>(std::stoi(roleStr));

        User loadedUser(userId, name, email, role);
        users.push_back(loadedUser);

    }

    inFile.close();
}

void Library::display_users() const {
    for(const auto& user: users) {
        std::cout << "ID: " << user.get_userId() << " | Title: " << user.get_name() << "\n";
    }
}



bool Library::is_valid_transaction(const std::string& bookId, const std::string& userId) {
    for(auto it = transactions.rbegin(); it != transactions.rend(); ++it) {
        if(it->get_bookId() == bookId && it->get_userId() == userId && it->get_type() == TransactionType::ISSUED) {
            return true;
        }
    }
    return false;
}