#include <iostream>
#include "models/Book.h"

int main() {
    Book myBook("INF-101", "C++ Primer", "Stanley Lippman", 49.99f);
    std::cout << "System restored! Book: " << myBook.get_title() << " is ready." << std::endl;
    return 0;
}