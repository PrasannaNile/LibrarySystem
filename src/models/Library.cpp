#include "models/Library.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>


// ************** System booting ***************

Library::Library() {
    load_books_from_file();
    load_user_from_file();
    load_transactions_from_file();
}



// ************** Books ***************

void Library::add_book(const Book& new_book) { 
    books.push_back(new_book); 
    save_books_to_file();
}


void Library::issue_book(const std::string& bookId, User& user) {
    for(auto& book: books) {
        if(book.get_bookId() == bookId) {
            BookStatus curr_status = book.get_status();
            if(curr_status == BookStatus::AVAILABLE) {

                transactions.push_back(Transaction(bookId, user.get_userId(), TransactionType::ISSUED));
                save_transactions_to_file();

                book.set_status(BookStatus::LOANED);
                active_loans[bookId] = user.get_userId();
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

                    transactions.push_back(Transaction(bookId, user.get_userId(), TransactionType::RETURNED));
                    save_transactions_to_file();
                    
                    book.set_status(BookStatus::AVAILABLE);
                    active_loans.erase(bookId);
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
        if (line.empty() || line == "\r" || line == "\n") {
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

        if (!statusStr.empty() && statusStr.back() == '\r') {
        statusStr.pop_back();
        }

        // 4. Bulletproof Validation Check before passing to stoi
        if (statusStr.empty() || statusStr.find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << "Warning: Skipped corrupted transaction row with invalid type field: '" << statusStr << "'\n";
            continue; 
        }

        float price = std::stof(priceStr);
        BookStatus status = static_cast<BookStatus> (std::stoi(statusStr));
        
        Book loadedBook(bookId, title, author, price, status);
        books.push_back(loadedBook);

        if(status == BookStatus::LOANED) active_loans[bookId];

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


void Library::display_active_loans() const {
    if (active_loans.empty()) {
        std::cout << "No books currently loaned!\n";
        return;
    }

    for (const auto& pair : active_loans) {
        std::cout << "User ID: " << pair.second 
                  << " | Holds Book ID: " << pair.first << "\n";
    }
}




// ************** Users ***************

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
        if(line.empty() || line == "\r" || line == "\n") continue;

        std::stringstream ss(line);

        std::string userId;
        std::string name;
        std::string email;
        std::string roleStr;

        std::getline(ss, userId, '|');
        std::getline(ss, name, '|');
        std::getline(ss, email, '|');
        std::getline(ss, roleStr, '|');

        if (!roleStr.empty() && roleStr.back() == '\r') {
        roleStr.pop_back();
        }

        // 4. Bulletproof Validation Check before passing to stoi
        if (roleStr.empty() || roleStr.find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << "Warning: Skipped corrupted transaction row with invalid type field: '" << roleStr << "'\n";
            continue; 
        }

        UserRole role = static_cast<UserRole>(std::stoi(roleStr));

        User loadedUser(userId, name, email, role);
        users.push_back(loadedUser);

    }

    inFile.close();
}


void Library::search_user(const std::string& userId) const {
    for(const auto& user: users) {
        if(user.get_userId() == userId) {
            std::cout << user.get_userId() << "| " << user.get_name() << "| " 
                      << user.get_email() << "| " << static_cast<int> (user.get_role());
            return;
        }
    }

    std::cerr << "Error: User " << userId << " does not exist!";
}


User* Library::search_user_by_id(const std::string& userId) {
    for(auto& user: users) {
        if(user.get_userId() == userId) return &user;
    }

    return nullptr;
}


void Library::display_users() const {
    for(const auto& user: users) {
        std::cout << "ID: " << user.get_userId() << " | Title: " << user.get_name() << "\n";
    }
}



// ************** Transactions ***************

bool Library::is_valid_transaction(const std::string& bookId, const std::string& userId) {


    for(auto it = transactions.rbegin(); it != transactions.rend(); ++it) {
        if(it->get_bookId() == bookId && it->get_userId() == userId && it->get_type() == TransactionType::ISSUED) {
            return true;
        }
    }
    return false;
}


void Library::save_transactions_to_file() const {
    std::ofstream outFile("transaction.txt");
    if(!outFile) {
        std::cerr << "Error: Denied permission or file not exist..\n";
        return;
    };

    for(const auto& txn: transactions) {
        outFile << txn.get_transactionId() << "|" << txn.get_bookId() << "|" 
                << txn.get_userId() << "|" << static_cast<int> (txn.get_type()) << "\n";
    }

    outFile.close();
}


void Library::load_transactions_from_file() {
    std::ifstream inFile("transaction.txt");
    if(!inFile) {
        std::cerr << "Error: Denied permission or file not exist..\n";
        return;
    }

    std::string line{};

    while(std::getline(inFile, line)) {
        if(line.empty() || line == "\r" || line == "\n") continue;

        std::stringstream ss(line);

        std::string transactionId{};
        std::string bookId{};
        std::string userId{};
        std::string typeStr{};

        std::getline(ss, transactionId, '|');
        std::getline(ss, bookId, '|');
        std::getline(ss, userId, '|');
        std::getline(ss, typeStr);

        if (!typeStr.empty() && typeStr.back() == '\r') {
            typeStr.pop_back();
        }

        // 4. Bulletproof Validation Check before passing to stoi
        if (typeStr.empty() || typeStr.find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << "Warning: Skipped corrupted transaction row with invalid type field: '" << typeStr << "'\n";
            continue; 
        }

        TransactionType type{ static_cast<TransactionType> (std::stoi(typeStr)) };

        Transaction txn(transactionId, bookId, userId, type);
        transactions.push_back(txn);

        if (type == TransactionType::ISSUED) {
            active_loans[bookId] = userId;
        } else if (type == TransactionType::RETURNED) {
            active_loans.erase(bookId);
        }

    }

}
