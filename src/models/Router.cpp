#include "models/Router.h"



Router::Router()
    : lib{}
{}



void Router::setDatabase(DBManager& dbManager) {
    db = &dbManager;
}


bool Router::authenticate() {
    
    int choice{1};

    while(true) {
        std::cout << "\n===============================\n";
        std::cout << "1. Login\n";
        std::cout << "2. Signup\n";
        std::cout << "0. Exit System Completely\n";
        std::cout << "===============================\n";
        
        std::cout << "Enter your choice: "; 
        std::cin >> choice;

        if (handleNumericInput()) continue;

        if(choice == 1) {
            return handleUserLogin();
        }
        else if(choice == 2) {
            return handleUserSignup();
        } else {
            std::cout << "System Shut down!\n";
            exit(0);
        }
    }

    
    
    return false;
        
}


bool Router::handleUserLogin() {
    clearInputBuffer();

    std::string userId{};
    std::cout << "UserId: ";
    std::getline(std::cin, userId);

    std::string email{};
    std::cout << "Email: ";
    std::getline(std::cin, email);

    User* user = lib.search_user_by_id(userId);

    if(user && user->get_userId() == userId && user->get_email() == email) {
        std::cout << "Successful login\n";
        currentUser = user;
        return true;
    } else {
        std::cout << "Invalid userId!\n";
    }

    return false;
}


bool Router::handleUserSignup() {
    clearInputBuffer();

    std::string name;
    std::string email;
    
    std::cout << "Enter name: ";
    std::getline(std::cin, name);

    std::cout << "Enter email: ";
    std::getline(std::cin, email);

    if(lib.is_email_registered(email)) {
        std::cout << "Email already exist!\n";
        return false;
    };

    User new_user(name, email, UserRole::STUDENT);
    currentUser = lib.register_user(new_user);

    std::cout << "Successful signup.\n";

    return true;
}


bool Router::handleNumericInput() {
    if(std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid input! Please enter a valid number.\n";
        return true;
    }

    return false;
}


void Router::clearInputBuffer() { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }


void Router::show_admin_menu() const {
    // ******* Menu *******
    std::cout << "1. Add Book\n";
    std::cout << "2. Display All Books\n";
    std::cout << "3. Register User\n";
    std::cout << "4. Display All Users\n";
    std::cout << "5. Search Book\n";
    std::cout << "6. Search User\n";
    std::cout << "0. Logout\n";
}


void Router::show_student_menu() const {
    std::cout << "1. Display All Books\n";
    std::cout << "2. Search Book\n";
    std::cout << "3. Borrow / Issue a Book\n";
    std::cout << "4. Return a Book\n";
    std::cout << "5. View My Borrowed Books Portfolio\n"; // Day 10 Module 1 hook
    std::cout << "0. Logout\n";
}


void Router::handleAdminRouter(int choice) {
    switch(choice) {
        case 1: {
            clearInputBuffer();
            std::string title;
            std::cout << "Title: "; std::getline(std::cin, title);

            std::string author;
            std::cout << "Author: "; std::getline(std::cin, author);

            std::string priceStr;
            std::cout << "Price: "; std::getline(std::cin, priceStr);
            
            float price{};
            try {
                price = std::stof(priceStr);
                lib.add_book(Book(title, author, price)); 
                std::cout << "Book added successfully!\n";
            } 
            catch (const std::exception& e) {
                std::cout << "Invalid price format! Please try again.\n";
            }

            break;
        }
        case 2:
            lib.display_books();
            break;
        case 3: {
            clearInputBuffer();
            std::string name;
            std::cout << "Name: "; 
            std::getline(std::cin, name);

            std::string email;
            std::cout << "Email: "; 
            std::getline(std::cin, email);

            if(lib.is_email_registered(email)) {
                std::cout << "Email already exist!\n";
                break;
            };

            std::string roleStr;
            std::cout << "Role(ADMIN: 1 , STUDENT: 2): "; 
            std::getline(std::cin, roleStr);

            int role{2};
            try {
                role = std::stoi(roleStr);
                if(role != 1 && role != 2) throw std::runtime_error("Invalid role selected");
                User* new_user = lib.register_user(User(name, email, static_cast<UserRole> (role)));

                std::cout << "User added successfully!\n";
            } 
            catch(const std::exception& e) {
                std::cout << "Invalid role format! Please try again.\n";
            }
            break;
        }
        case 4:
            lib.display_users();
            break;
        case 5: {
            clearInputBuffer();

            std::cout << "Title/Author of book: ";
            std::string searchQuery{};
            std::getline(std::cin, searchQuery);

            lib.search_book(searchQuery);
            break;
        }
        case 6: {
            clearInputBuffer();

            std::cout << "UserId required: ";
            std::string searchQuery{};
            std::getline(std::cin, searchQuery);

            lib.search_user(searchQuery);
            break;
        }
        case 0:
            std::cout << "Exiting system...\n";
            exit(0); // Safely terminates the app
        default:
            std::cout << "Invalid Admin choice!\n";
    }
}


void Router::handleStudentRouter(int choice) {
    switch(choice) {
        case 1:
            lib.display_books(); // For students, 1 displays books!
            break;
        
        case 2: {
            clearInputBuffer();
            std::string searchQuery{};
            std::cout << "Title/Author of book: ";
            std::getline(std::cin, searchQuery);

            lib.search_book(searchQuery);
            break;
        }

        case 3: {
            clearInputBuffer();
            std::cout << "BookId: ";
            std::string bookId{};
            std::getline(std::cin, bookId);

            lib.issue_book(bookId, *currentUser);
            break;
        }

        case 4: {
            clearInputBuffer();
            std::cout << "BookId: ";
            std::string bookId{};
            std::getline(std::cin, bookId);

            lib.return_book(bookId, *currentUser);
            break;
        }

        case 5: 
            currentUser->display_borrowed_books();
            break;

        case 0:
            std::cout << "Exiting system...\n";
            exit(0);
        default:
            std::cout << "Invalid Student choice!\n";
    }
}


void Router::run() {
    if (db == nullptr) {
        std::cerr << "[Router Error] Database reference is missing!\n";
        return;
    }

    if (currentUser == nullptr) return;

    while (true) {
        if (currentUser->get_role() == UserRole::ADMIN) {
            show_admin_menu();
        } else {
            show_student_menu();
        }

        int choice{};
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (handleNumericInput()) continue;
        if (choice == 0) break; // Exit application boundary

        if (currentUser->get_role() == UserRole::ADMIN) {
            handleAdminRouter(choice);
        } else {
            handleStudentRouter(choice);
        }
    }
}