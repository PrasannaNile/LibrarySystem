CREATE DATABASE IF NOT EXISTS LibrarySystem;
USE LibrarySystem;

DROP TABLE IF EXISTS Transaction;
DROP TABLE IF EXISTS User;
DROP TABLE IF EXISTS Book;


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
    price DECIMAL(4, 2),
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

INSERT INTO USER (userId, name, email, role)
VALUES ("U1001", "Admin", "admin@gmail.com", "ADMIN");


# ******************* Borrowed_Books Schema *******************
-- CREATE TABLE IF NOT EXISTS ActiveLoans (
-- 	
-- );
# ******************* Borrowed_Books Schema *******************

SELECT * FROM User;
SELECT * FROM Book;
SELECT * FROM Transaction;

INSERT INTO BOOK
VALUES ("B1001", "BOOK1", "AUTHOR1", 89.99, "AVAILABLE");

DROP TABLE BOOK;
DROP TABLE TRANSACTION;