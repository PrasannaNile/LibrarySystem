# pragma once

#include <string>


enum class TransactionType {ISSUED = 1, RETURNED = 2};

class Transaction {
private:
    std::string transactionId;
    std::string bookId;
    std::string userId;
    TransactionType type;
    static int countId;

public:
    Transaction(const std::string& bookId, const std::string& userId, const TransactionType type);
    Transaction(const std::string& transactionId, const std::string& bookId, const std::string& userId, const TransactionType type);
    std::string get_transactionId() const;
    std::string get_bookId() const;
    std::string get_userId() const;
    TransactionType get_type() const;
    static void id_incrementor();
};