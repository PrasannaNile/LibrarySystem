# pragma once

#include <string>


enum class TransactionType {ISSUED, RETURNED};

class Transaction {
private:
    std::string transactionId;
    std::string bookId;
    std::string userId;
    TransactionType type;

public:
    Transaction(const std::string& transactionId, const std::string& bookId, const std::string& userId, const TransactionType type);
    std::string get_transactionId() const;
    std::string get_bookId() const;
    std::string get_userId() const;
    TransactionType get_type() const;
};