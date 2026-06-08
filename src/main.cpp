#include "models/Library.h"
#include <iostream>

int main() {
    Library lib;

    // 1. Setup Data
    lib.add_book(Book("B01", "The C++ Programming Language", "abc1", 500.50));
    lib.add_book(Book("B02", "Effective Modern C++", "abc2", 899.00));
    lib.register_user(User("U101", "Alice", "alice@gmail.com", UserRole::STUDENT));
    lib.register_user(User("U102", "Bob", "bob@gmail.com", UserRole::STUDENT));

    std::cout << "--- STARTING SYSTEM TEST ---\n\n";

    // Test Case 1: Standard Valid Issue
    std::cout << "[Test 1] Alice tries to borrow B01:\n";
    lib.issue_book("B01", "U101"); 
    std::cout << "\n";

    // Test Case 2: Double Issue Prevention
    std::cout << "[Test 2] Bob tries to borrow B01 while Alice has it:\n";
    lib.issue_book("B01", "U102"); 
    std::cout << "\n";

    // Test Case 3: Wrong User Return Prevention
    std::cout << "[Test 3] Bob tries to return B01 (which Alice holds):\n";
    lib.return_book("B01", "U102"); 
    std::cout << "\n";

    // Test Case 4: Valid Return
    std::cout << "[Test 4] Alice returns B01:\n";
    lib.return_book("B01", "U101"); 
    std::cout << "\n";

    // Test Case 5: Re-issue After Return (Testing the Append Ledger!)
    std::cout << "[Test 5] Bob tries to borrow B01 now that it's back:\n";
    lib.issue_book("B01", "U102"); 
    std::cout << "\n";

    std::cout << "--- TEST SUITE COMPLETE ---\n";
    return 0;
}