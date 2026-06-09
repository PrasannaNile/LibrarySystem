#include "models/Book.h"

#include <iostream>


int Book::countId = 1001;


// constructor with auto generate Id
Book::Book(const std::string& _title, const std::string& _author, const float _price)
    : title{_title}, author{_author}, price{_price}, status{BookStatus::AVAILABLE}
{
    this->bookId = "B" + std::to_string(countId);
    id_incrementor();
    profileLog = new std::string("Initial Log");
}


// constructor with Id parameter
Book::Book(const std::string& parsedId, const std::string& _title, const std::string& _author, const float _price, const BookStatus& _status) 
{
    // Force the object to take the EXACT text string written in your file
    this->bookId = parsedId;  
    this->title = _title;
    this->author = _author;
    this->price = _price;
    this->status = _status;

    // Synchronize your counter so fresh books don't collide
    int numericId = std::stoi(parsedId.substr(1)); // e.g., "B1002" -> 1002
    if (numericId >= countId) {
        countId = numericId + 1; // countId pushes to 1003
    }

    profileLog = new std::string("Initial Log");
}

// copy constructor
Book::Book(const Book& other) {
    this->bookId = other.bookId;       // Copy the ID exactly as it is! Do NOT generate a new one.
    this->title = other.title;
    this->author = other.author;
    this->price = other.price;
    this->status = other.status;
    
    // Handle your deep copy safely without messing with counters
    this->profileLog = new std::string(*other.profileLog); 
}

// copy assignment constructor
Book& Book::operator=(const Book& other) {
    if(this == &other) return *this;

    delete profileLog;
    profileLog = new std::string(*other.profileLog);

    title = other.title;
    author = other.author;
    price = other.price;
    status = ::BookStatus::AVAILABLE;

    return *this;
}


Book::~Book() {
    delete profileLog;
}


void Book::id_incrementor() { countId++; }
std::string Book::get_bookId() const { return this->bookId; }
std::string Book::get_title() const { return this->title; }
std::string Book::get_author() const { return this->author; }
float Book::get_price() const { return this->price; }
BookStatus Book::get_status() const { return this->status; }

void Book::set_status(BookStatus new_status) { this->status = new_status; }