#include "models/Book.h"

Book::Book(std::string _bookId, std::string _title, std::string _author, float _price)
    : bookId{_bookId}, title{_title}, author{_author}, price{_price}, status{BookStatus::AVAILABLE} 
{}

std::string Book::get_bookId() const { return bookId; }
std::string Book::get_title() const { return title; }
std::string Book::get_author() const { return author; }
float Book::get_price() const { return price; }
BookStatus Book::get_status() const { return status; }

void Book::set_status(BookStatus new_status) { status = new_status; }