#include "models/Book.h"


// constructor 
Book::Book(std::string _bookId, std::string _title, std::string _author, float _price)
    : bookId{_bookId}, title{_title}, author{_author}, price{_price}, status{::BookStatus::AVAILABLE}
{
    profileLog = new std::string("Initial Log");
}

// copy constructor
Book::Book(const Book& other) 
    : bookId{other.bookId}, title{other.title}, author{other.author}, price{other.price}, status{::BookStatus::AVAILABLE}
{
    profileLog = new std::string(*other.profileLog);
}

// copy assignment constructor
Book& Book::operator=(const Book& other) {
    if(this == &other) return *this;

    delete profileLog;
    profileLog = new std::string(*other.profileLog);

    bookId = other.bookId;
    title = other.title;
    author = other.author;
    price = other.price;
    status = ::BookStatus::AVAILABLE;

    return *this;
}


Book::~Book() {
    delete profileLog;
}

std::string Book::get_bookId() const { return bookId; }
std::string Book::get_title() const { return title; }
std::string Book::get_author() const { return author; }
float Book::get_price() const { return price; }
BookStatus Book::get_status() const { return status; }

void Book::set_status(BookStatus new_status) { status = new_status; }