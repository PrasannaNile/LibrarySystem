#include "models/Library.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>


// ************** System booting ***************

Library::Library(DBManager* database_ptr) {
    this->db = database_ptr;
    std::cout << "[Library] Initialized engine with active database link.\n";

    // load_books_from_file();
    // load_user_from_file();
    // load_transactions_from_file();
}



// ************** Books ***************

void Library::add_book(const Book& new_book) { 
    // save_books_to_file();

    if (this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Add Book Library Error] Cannot insert book. Database connection is null.\n";
        return;
    }

    std::stringstream queryStream;
    queryStream << "INSERT INTO Book (bookId, title, author, price, status) VALUES ("
                << "'" << new_book.get_bookId() << "', "
                << "'" << new_book.get_title() << "', "
                << "'" << new_book.get_author() << "', "
                << new_book.get_price() << ", " // Decimal field (no single quotes needed)
                << "'AVAILABLE');";

    std::string query = queryStream.str();

    if (mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to insert book into MariaDB: " 
                  << mysql_error(this->db->get_connection()) << "\n\n";
        return;
    }
    
    books.push_back(new_book); 
    std::cout << "[Library] Backup Success: Book '" << new_book.get_title() 
              << "' permanently saved under ID " << new_book.get_bookId() << ".\n";
}


void Library::issue_book(const std::string& bookId, User& user) {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot issue book. Database connection is null.\n";
        return;
    }

    std::stringstream queryStream;
    queryStream << "SELECT status FROM Book WHERE bookId = " << "'" << bookId << "'";
    std::string query = queryStream.str();

    if (mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to issue book into MariaDB: " 
                  << mysql_error(this->db->get_connection()) << "\n\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return;

    auto num_rows = mysql_num_rows(result);
    if (num_rows == 0) {
        std::cout << "\n====================================\n";
        std::cout << "[Library info] No books in Library!\n";
        std::cout << "====================================\n\n";
        mysql_free_result(result);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    std::string statusStr = row[0] ? row[0] : "AVAILABLE";
    std::cout << "status while issuing book: " << statusStr << "\n\n";
    
    BookStatus status = BookStatus::AVAILABLE;
    if(statusStr == "LOANED") status = BookStatus::LOANED;
    if(statusStr == "RESERVED") status = BookStatus::RESERVED;
    if(statusStr == "LOST") status = BookStatus::LOST;


    if(status != BookStatus::AVAILABLE) {
        std::cout << "[Book Status Error]: Book " << bookId << " is currently not available (Status: LOANED/RESERVED/LOST).\n";
        return;
    }

    // if borrowed books are already 3 then no book issued logic comes here
    if(get_borrowed_books_count(user.get_userId()) >= 3) {
        std::cerr << "[Library Error] Pending Books. Cannot issue one more book!\n\n";
        return;
    }

    // save logic of transaction
    std::string transactionId = generate_next_transaction_id();
    Transaction new_tran = Transaction(transactionId, bookId, user.get_userId(), TransactionType::ISSUED);
    save_transaction_to_db(new_tran);

    update_book_status(bookId, BookStatus::LOANED);

    // active loans logic comes here

    mysql_free_result(result);
    std::cout << "Success: Book " << bookId << " successfully issued to user " << user.get_userId() << ".\n";
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

                    Transaction new_tran = Transaction(bookId, user.get_userId(), TransactionType::RETURNED);
                    save_transaction_to_db(new_tran);
                    
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

// old file system
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

// old file system
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


void Library::load_books_from_database() {
    // 1. Safety check: make sure our database connection handle is valid
    if (this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot load books. Database connection is null.\n";
        return;
    }

    // 2. Fire the query string down the pipeline
    std::string query = "SELECT bookId, title, author, price, status FROM Book;";
    if (mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "[Library Error] Query Failed: " << mysql_error(this->db->get_connection()) << "\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if (result == nullptr) {
        return;
    }

    books.clear();

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != nullptr) {
        std::string bookId = row[0] ? row[0] : "";
        std::string title = row[1] ? row[1] : "";
        std::string author = row[2] ? row[2] : "";
        std::string priceStr = row[3] ? row[3] : "0.00";
        std::string statusStr = row[3] ? row[3] : "AVAILABLE";

        float price {};

        try {
            price = std::stof(priceStr);
        } catch(...) {
            price = 0.0f;
        }

        BookStatus status = BookStatus::AVAILABLE;
        if(statusStr == "LOANED") status = BookStatus::LOANED;
        else if(statusStr == "RESERVED") status = BookStatus::RESERVED;
        else if(statusStr == "LOST") status = BookStatus::LOST;

        Book loadedBook(bookId, title, author, price, status);
        books.push_back(loadedBook);

    }
    mysql_free_result(result);
    std::cout << "[Library] Successfully loaded " << books.size() << " books from MariaDB.\n";
}

// old file system
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


void Library::search_books_by_substr(std::string& searchQuery) {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot search books by searchQuery. Database connection is null.\n";
        return;
    }

    std::stringstream queryStream;
    queryStream << "SELECT bookId, title, author FROM Book "
                << "WHERE bookId LIKE '%" << searchQuery << "%'"
                << "OR title LIKE '%" << searchQuery << "%'"
                << "OR author LIKE '%" << searchQuery << "%';";

    std::string query = queryStream.str();
    if(mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "[Library Error] Query Failed: " << mysql_error(this->db->get_connection()) << "\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return;

    auto no_of_rows = mysql_num_rows(result);
    if(no_of_rows == 0) {
        std::cout << "[Library Error] No matching books for subQuery\n\n";
        return;
    }

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != 0) {
        std::cout << row[0] << "| " << row[1] << "| " << row[2] << "\n";
    }

    mysql_free_result(result);

}


Book* Library::search_book_by_id(const std::string& bookId) const {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot search book by id. Database connection is null.\n";
        return nullptr;
    }

    std::stringstream queryStream;
    queryStream << "SELECT bookId, title, author, price, status FROM Book WHERE bookId = '" << bookId << "';";
    std::string query = queryStream.str();

    if(mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to search book into MariaDB: " 
                  << mysql_error(this->db->get_connection()) << "\n\n";
        return nullptr;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return nullptr;

    auto no_of_rows = mysql_num_rows(result);
    if(no_of_rows == 0) {
        std::cerr << "[Library Error] No book with Id" << bookId << "\n";
        return nullptr;
    }
    MYSQL_ROW row;

    row = mysql_fetch_row(result);
    std::string bId = row[0] ? row[0] : "";
    std::string title = row[1] ? row[1] : "";
    std::string author = row[2] ? row[2] : "";
    std::string priceStr = row[3] ? row[3] : "";
    std::string statusStr = row[4] ? row[4] : "";

    BookStatus status = BookStatus::AVAILABLE;
    if(statusStr == "RESERVED") status = BookStatus::RESERVED;
    else if(statusStr == "LOANED") status = BookStatus::LOANED;
    else if(statusStr == "LOST") status = BookStatus::LOST;

    try {
        float price = std::stof(priceStr);
        Book* search_book = new Book(bId, title, author, price, status);

        std::cout << "Book with id " << bookId << " found\n";
        return search_book;
    } catch(const std::exception&) {}

    return nullptr;
}


void Library::display_books() const {
    
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot display books. Database connection is null.\n";
        return;
    }
    
    std::stringstream querystream;
    querystream << "SELECT bookId, title, author, price FROM Book;";
    
    std::string query = querystream.str();
    if(mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to display book into MariaDB: " 
        << mysql_error(this->db->get_connection()) << "\n\n";
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return;

    auto num_rows = mysql_num_rows(result);
    if (num_rows == 0) {
        std::cout << "\n====================================\n";
        std::cout << "[Library info] No books in Library!\n";
        std::cout << "====================================\n\n";
        mysql_free_result(result);
        return;
    }

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != 0) {
        std::cout << row[0] << "| " << row[1] << "| " << row[2] << "| " << row[3] << "\n";
    }

    mysql_free_result(result);
}


void Library::display_active_loans() const {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cout << "[Database Error] Cannot display active loans. Database connection is null.\n";
        return;
    }

    std::stringstream queryStream;
    queryStream << "SELECT bookId, userId "
                << "FROM Transaction T1 "
                << "WHERE type = 'ISSUED' "
                << "AND T1.transactionId NOT IN ("
                    << "SELECT T2.transactionId "
                    << "FROM Transaction T2 "
                    << "WHERE type = 'RETURNED' "
                    << "AND T1.transactionId < T2.transactionId"
                << ");";
    std::string query = queryStream.str();
    if(mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to display active book from MariaDB: " 
                  << mysql_error(this->db->get_connection()) << "\n\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return;

    auto no_of_rows = mysql_num_rows(result);
    if(no_of_rows == 0) {
        std::cerr << "No active books. All books are available\n\n";
        return;
    }

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != 0) {
        std::cout << row[0] << "| " << row[1] << "\n";
    }

    mysql_free_result(result);
}


void Library::update_book_status(const std::string& bookId, BookStatus status) {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot change book status. Database connection is null.\n";
        return;
    }

    std::stringstream queryStr;
    std::string statusStr = "AVAILABLE";
    if(status == BookStatus::LOANED) statusStr = "LOANED";
    else if(status == BookStatus::RESERVED) statusStr = "RESERVED";
    else if(status == BookStatus::LOST) statusStr = "LOST";

    queryStr << "UPDATE Book SET status = " << "'" << statusStr << "' " << "WHERE bookId = " << "'" << bookId << "';";
    std::string query = queryStr.str();

    if(mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to display book into MariaDB: " 
        << mysql_error(this->db->get_connection()) << "\n\n";
        return;
    }

    std::cout << "[Database Info] Book " << bookId << " status successfully changed to '" << statusStr << "'.\n";
    
}


std::string Library::generate_next_book_id() {
    if (this->db == nullptr || this->db->get_connection() == nullptr) return "B1001";

    std::string query = "SELECT bookId FROM Book ORDER BY bookId DESC LIMIT 1;";
    if (mysql_query(this->db->get_connection(), query.c_str()) != 0) return "B1001";

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if (result == nullptr) return "B1001";

    std::string nextId = "B1001";
    if (mysql_num_rows(result) > 0) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row[0]) {
            std::string lastId = row[0]; // e.g., "B1002"
            int numericPart = std::stoi(lastId.substr(1));
            nextId = "B" + std::to_string(numericPart + 1);
        }
    }
    mysql_free_result(result);
    return nextId;
}


int Library::get_borrowed_books_count(const std::string& userId) const {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot count borrow book. Database connection is null.\n";
        return 0;
    }

    std::stringstream queryStream;
    queryStream << "SELECT COUNT(DISTINCT t1.bookId) "
                << "FROM Transaction t1 "
                << "WHERE t1.userId = " << "'" << userId << "' "  
                << "AND t1.type = 'ISSUED' " 
                << "AND t1.bookId NOT IN ( "
                <<    "SELECT t2.bookId " 
                <<    "FROM Transaction t2 " 
                << "    WHERE t2.userId = '" << userId << "' "
                <<        "AND t2.type = 'RETURNED' " 
                <<        "AND t2.transactionId > t1.transactionId "
                << ");";
    std::string query = queryStream.str();

    if(mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to count borrow book into MariaDB: " 
                  << mysql_error(this->db->get_connection()) << "\n\n";
        return 0;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return 0;

    int count = 0;
    if (mysql_num_rows(result) > 0) {
        MYSQL_ROW row = mysql_fetch_row(result);
        count = row[0] ? std::stoi(row[0]) : 0;
    }

    mysql_free_result(result);
    return count;
}


void Library::display_borrow_books(const std::string& userId) const {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot count borrow book. Database connection is null.\n";
        return;
    }

    std::stringstream queryStream;
    queryStream << "SELECT t1.bookId "
                << "FROM Transaction t1 "
                << "WHERE t1.userId = " << "'" << userId << "' "  
                << "AND t1.type = 'ISSUED' " 
                << "AND t1.bookId NOT IN ( "
                <<    "SELECT t2.bookId " 
                <<    "FROM Transaction t2 " 
                << "    WHERE t2.userId = '" << userId << "' "
                <<        "AND t2.type = 'RETURNED' " 
                <<        "AND t2.transactionId > t1.transactionId "
                << ");";
    std::string query = queryStream.str();

    if(mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to insert book into MariaDB: " 
                  << mysql_error(this->db->get_connection()) << "\n\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return;

    auto no_of_rows = mysql_num_rows(result);
    if(no_of_rows == 0) {
        std::cout << "[Library Error] No active books are borrowed!\n\n";
    }

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != 0) {
        std::string bookId = row[0] ? row[0] : "";
        Book* book = search_book_by_id(bookId);

        if(book) {
            std::cout << book->get_bookId() << "| " << book->get_title() << "| "
                      << book->get_author() << "| " << book->get_price() << "\n";
        }
    }

    mysql_free_result(result);

}



// ************** Users ***************

User* Library::register_user(const User& new_user) { 
    // save_user_to_file();

    if (this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot register user. Database connection is null.\n";
        return nullptr;
    }

    std::stringstream queryStream;
    queryStream << "INSERT INTO User (userId, name, email, role) VALUES ("
                << "'" << new_user.get_userId() << "', "
                << "'" << new_user.get_name() << "', "
                << "'" << new_user.get_email() << "', "
                << "'STUDENT');";

    std::string query = queryStream.str();

    if (mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Registration failed: " 
                  << mysql_error(this->db->get_connection()) << "\n\n";
        return nullptr;
    }

    users.push_back(new_user); 
    registered_emails[new_user.get_email()] = true;

    std::cout << "[Library] Backup Success: Student '" << new_user.get_name() 
              << "' permanently registered under ID " << new_user.get_userId() << ".\n";

    return &users.back();
}

// old file system
bool Library::is_email_registered(const std::string& email) const {
    return registered_emails.find(email) != registered_emails.end();
}

// old file system
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

// old file system
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
        registered_emails[email] = true;
        users.push_back(loadedUser);

    }

    inFile.close();
}


void Library::load_users_from_database() {
    if (this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot load users. Database connection is null.\n";
        return;
    }

    // 2. Fire the query string down the pipeline
    std::string query = "SELECT userId, name, email, role FROM User;";
    if (mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "[Library Error] Query Failed: " << mysql_error(this->db->get_connection()) << "\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return;

    users.clear();

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != nullptr) {
        std::string userId = row[0] ? row[0] : "";
        std::string name = row[1] ? row[1] : "";
        std::string email = row[2] ? row[2] : "";
        std::string roleStr = row[3] ? row[3] : "STUDENT";

        UserRole role = UserRole::STUDENT;
        if(roleStr == "ADMIN") role = UserRole::ADMIN;

        User loadedUser(userId, name, email, role);
        users.push_back(loadedUser);

    }
    mysql_free_result(result);
    std::cout << "[Library] Successfully loaded " << users.size() << " users from MariaDB.\n";

}


void Library::search_user_by_substr(std::string& searchQuery) const {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot search user by searchQuery. Database connection is null.\n";
        return;
    }

    std::stringstream queryStream;
    queryStream << "SELECT userId, name FROM User "
                << "WHERE userId LIKE '%" << searchQuery << "%'"
                << "OR name LIKE '%" << searchQuery << "%';";

    std::string query = queryStream.str();
    if(mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "[Library Error] Query Failed: " << mysql_error(this->db->get_connection()) << "\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return;

    auto no_of_rows = mysql_num_rows(result);
    if(no_of_rows == 0) {
        std::cout << "[Library Error] No matching users for subQuery\n\n";
        return;
    }

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != 0) {
        std::cout << row[0] << "| " << row[1] << "\n";
    }

    mysql_free_result(result);
}


User* Library::search_user_by_id(const std::string& userId) {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot search user. Database connection is null.\n";
        return nullptr;
    }

    std::stringstream queryStream;
    queryStream << "SELECT userId, name, email, role FROM User WHERE userId = '" << userId << "';";
    std::string query = queryStream.str();

    if (mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to search user into MariaDB: " 
                  << mysql_error(this->db->get_connection()) << "\n\n";
        return nullptr;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return nullptr;

    auto num_rows = mysql_num_rows(result);
    if (num_rows == 0) {
        std::cout << "\n====================================\n";
        std::cout << "[Library info] No User in Library with userId: " << userId << "\n";
        std::cout << "====================================\n\n";
        mysql_free_result(result);
        return nullptr;
    }
        

    MYSQL_ROW row;
    row = mysql_fetch_row(result);

    std::string uId = row[0] ? row[0] : "";
    std::string name = row[1] ? row[1] : "";
    std::string email = row[2] ? row[2] : "";
    std::string roleStr = row[3] ? row[3] : "";

    UserRole role = UserRole::STUDENT;
    if(roleStr == "ADMIN") role = UserRole::ADMIN;

    User* searched_user = new User(uId, name, email, role);
    return searched_user;
}


void Library::display_users() const {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot display user. Database connection is null.\n";
        return;
    }

    std::stringstream queryStream;
    queryStream << "SELECT userId, name, email, role FROM User;";
    std::string query = queryStream.str();

    if(mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to display book into MariaDB: " 
        << mysql_error(this->db->get_connection()) << "\n\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return;

    auto num_rows = mysql_num_rows(result);
    if (num_rows == 0) {
        std::cout << "\n====================================\n";
        std::cout << "[Library info] No books in Library!\n";
        std::cout << "====================================\n\n";
        mysql_free_result(result);
        return;
    }

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != 0) {
        std::cout << row[0] << "| " << row[1] << "| " << row[2] << "| " << row[3] << "\n";
    }

    mysql_free_result(result);
    
}


std::string Library::generate_next_user_id() {
    if (this->db == nullptr || this->db->get_connection() == nullptr) return "U1001";

    // Grab the highest alphabetical ID from the database
    std::string query = "SELECT userId FROM User ORDER BY userId DESC LIMIT 1;";
    if (mysql_query(this->db->get_connection(), query.c_str()) != 0) return "U1001";

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if (result == nullptr) return "U1001";

    std::string nextId = "U1001"; // Fallback default
    if (mysql_num_rows(result) > 0) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row[0]) {
            std::string lastId = row[0]; // e.g., "U1005"
            // Extract the digits following the 'U' character
            int numericPart = std::stoi(lastId.substr(1)); 
            nextId = "U" + std::to_string(numericPart + 1);
        }
    }
    mysql_free_result(result);
    return nextId;
}



// ************** Transactions ***************
// old file system
bool Library::is_valid_transaction(const std::string& bookId, const std::string& userId) {


    for(auto it = transactions.rbegin(); it != transactions.rend(); ++it) {
        if(it->get_bookId() == bookId && it->get_userId() == userId && it->get_type() == TransactionType::ISSUED) {
            return true;
        }
    }
    return false;
}

// old file system
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


void Library::save_transaction_to_db(const Transaction& new_trans) {
    // save_transactions_to_file();
    if (this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot register user. Database connection is null.\n";
        return;
    }

    std::string typeStr = "ISSUED";
    if (new_trans.get_type() == TransactionType::RETURNED) {
        typeStr = "RETURNED";
    }

    std::stringstream queryStream;
    queryStream << "INSERT INTO Transaction (transactionId, bookId, userId, type) VALUES ("
                << "'" << new_trans.get_transactionId() << "', "
                << "'" << new_trans.get_bookId() << "', "
                << "'" << new_trans.get_userId() << "', "
                << "'" << typeStr << "');";

    std::string query = queryStream.str();

    if(mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "\n[Database Error] Failed to record transaction in MariaDB: " 
                  << mysql_error(this->db->get_connection()) << "\n\n";
        return;
    }

    transactions.push_back(new_trans);
    std::cout << "[Library] Backup Success: Transaction " << new_trans.get_transactionId() 
              << " securely committed to database.\n";
}

// old file system
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


void Library::load_transactions_from_database() {
    if(this->db == nullptr || this->db->get_connection() == nullptr) {
        std::cerr << "[Library Error] Cannot load transactions. Database connection is null.\n";
        return;
    }

    std::string query = "SELECT transactionId, bookId, userId, type FROM Transaction;";
    if (mysql_query(this->db->get_connection(), query.c_str()) != 0) {
        std::cerr << "[Library Error] Query Failed: " << mysql_error(this->db->get_connection()) << "\n";
        return;
    }

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if(result == nullptr) return;

    transactions.clear();

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != nullptr) {
        std::string transactionId = row[0] ? row[0] : "";
        std::string bookId = row[1] ? row[1] : "";
        std::string userId = row[2] ? row[2] : "";
        std::string typeStr = row[3] ? row[3] : "STUDENT";

        TransactionType type = TransactionType::ISSUED;
        if(typeStr == "RETURNED") type = TransactionType::RETURNED;

        Transaction loadedTransaction(transactionId, bookId, userId, type);
        transactions.push_back(loadedTransaction);

    }
    mysql_free_result(result);
    std::cout << "[Library] Successfully loaded " << transactions.size() << " transactions from MariaDB.\n";
}


std::string Library::generate_next_transaction_id() {
    if (this->db == nullptr || this->db->get_connection() == nullptr) return "T1001";

    std::string query = "SELECT transactionId FROM Transaction ORDER BY transactionId DESC LIMIT 1;";
    if (mysql_query(this->db->get_connection(), query.c_str()) != 0) return "T1001";

    MYSQL_RES* result = mysql_store_result(this->db->get_connection());
    if (result == nullptr) return "T1001";

    std::string nextId = "T1001";
    if (mysql_num_rows(result) > 0) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row[0]) {
            std::string lastId = row[0]; // e.g., "T1001"
            int numericPart = std::stoi(lastId.substr(1));
            nextId = "T" + std::to_string(numericPart + 1);
        }
    }
    mysql_free_result(result);
    return nextId;
}
