#pragma once
#include <string>

enum class BookStatus { AVAILABLE, LOANED, RESERVED, LOST };

class Book {
private:
    std::string bookId;
    std::string title;
    std::string author;
    float price;
    BookStatus status;

public:
    Book(std::string _bookId, std::string _title, std::string _author, float _price);
    
    std::string get_bookId() const;
    std::string get_title() const;
    std::string get_author() const;
    float get_price() const;
    BookStatus get_status() const;

    void set_status(BookStatus new_status);
};