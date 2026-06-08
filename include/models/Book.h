#pragma once
#include <string>

enum class BookStatus { AVAILABLE = 1, LOANED = 2, RESERVED = 3, LOST = 4 };

class Book {
private:
    std::string bookId;
    std::string title;
    std::string author;
    float price;
    BookStatus status;
    std::string* profileLog;


public:
    Book(std::string _bookId, std::string _title, std::string _author, float _price);
    Book(const Book& other);
    Book& operator=(const Book& other);
    ~Book();
    
    std::string get_bookId() const;
    std::string get_title() const;
    std::string get_author() const;
    float get_price() const;
    BookStatus get_status() const;

    void set_status(BookStatus new_status);
};