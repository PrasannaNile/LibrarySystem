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
