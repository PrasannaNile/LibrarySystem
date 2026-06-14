CREATE DATABASE IF NOT EXISTS LibrarySystem;
USE LibrarySystem;

# ******************* User Schema *******************
CREATE TABLE IF NOT EXISTS User (
	userId VARCHAR(50) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(50) UNIQUE NOT NULL,
    role ENUM("ADMIN", "STUDENT") NOT NULL
);
# ******************* User Schema ******************* 


# ******************* Book Schema *******************
CREATE TABLE IF NOT EXISTS Book (
	bookId VARCHAR(50) PRIMARY KEY,
    title VARCHAR(100) NOT NULL,
    author VARCHAR(100) NOT NULL,
    status ENUM("AVAILABLE", "LOANED", "RESERVED", "LOST") NOT NULL DEFAULT "AVAILABLE"
);
# ******************* Book Schema *******************


# ******************* Transaction Schema *******************
CREATE TABLE IF NOT EXISTS Transaction (
	transactionId VARCHAR(50) PRIMARY KEY,
    bookId VARCHAR(50) NOT NULL,
    userId VARCHAR(50) NOT NULL,
    type ENUM("ISSUED", "RETURNED") NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (bookId) REFERENCES Book(bookId),
    FOREIGN KEY (userId) REFERENCES User(userId)
); 
# ******************* Transaction Schema *******************

SHOW TABLES;