#include "models/Library.h"
#include <iostream>
#include <limits>
#include <limits.h>

bool handleNumericInput() {
    if(std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid input! Please enter a valid number.\n";
        return true;
    }

    return false;
}

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


int main() {
    Library lib;

    while(true) {
        std::cout << "<--- Library Management System --->\n";
        // ******* Menu *******
        std::cout << "1. Add Book\n";
        std::cout << "2. Display All Books\n";
        std::cout << "3. Register User\n";
        std::cout << "4. Display All Users\n";
        std::cout << "0. Exit\n";

        // ***** user choice *****
        int choice{};
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        if(handleNumericInput()) continue;


        // ***** choice evaluation *****
        if(choice == 0) break;
        
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

            case 3:
                // lib.register_user(User())
                break;

            case 4:
                lib.display_users();
                break;

            case 0:
                exit(0);

            default:
                std::cout << "Invalid choice, please try again.\n";
 
        }


    }


    return 0;
}