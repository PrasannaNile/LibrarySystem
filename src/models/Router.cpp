#include "models/Router.h"



Router::Router()
    : lib{}, currentUser{"Student", "student@lib.com", UserRole::STUDENT}
{}


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
    std::cout << "0. Exit\n";
}


void Router::show_student_menu() const {
    std::cout << "1. Display All Books\n";
    std::cout << "2. Search Book\n";
    std::cout << "3. Borrow / Issue a Book\n";
    std::cout << "4. Return a Book\n";
    std::cout << "5. View My Borrowed Books Portfolio\n"; // Day 10 Module 1 hook
    std::cout << "0. Exit\n";
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
            std::cout << "Name: "; std::getline(std::cin, name);

            std::string email;
            std::cout << "Email: "; std::getline(std::cin, email);

            std::string roleStr;
            std::cout << "Role(ADMIN: 1 , STUDENT: 2): "; std::getline(std::cin, roleStr);

            int role{2};
            try {
                role = std::stoi(roleStr);
                if(role != 1 && role != 2) throw std::runtime_error("Invalid role selected");
                lib.register_user(User(name, email, static_cast<UserRole> (role)));
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

            lib.issue_book(bookId, currentUser);
            break;
        }

        case 4: {
            clearInputBuffer();
            std::cout << "BookId: ";
            std::string bookId{};
            std::getline(std::cin, bookId);

            lib.return_book(bookId, currentUser);
            break;
        }

        case 5: 
            currentUser.display_borrowed_books();
            break;

        case 0:
            std::cout << "Exiting system...\n";
            exit(0);
        default:
            std::cout << "Invalid Student choice!\n";
    }
}


void Router::run() {
    while (true) {
        std::cout << "<--- Library Management System --->\n";

        if (currentUser.get_role() == UserRole::ADMIN) {
            show_admin_menu();
        } else {
            show_student_menu();
        }

        int choice{};
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (handleNumericInput()) continue;
        if (choice == 0) break; // Exit application boundary

        if (currentUser.get_role() == UserRole::ADMIN) {
            handleAdminRouter(choice);
        } else {
            handleStudentRouter(choice);
        }
    }
}