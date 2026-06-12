#include "models/Transaction.h"


int Transaction::countId = 1001;


Transaction::Transaction(const std::string& transactionId, const std::string& bookId, const std::string& userId, const TransactionType type)
    : transactionId{transactionId}, bookId{bookId}, userId{userId}, type{type}
{
    int numericId = std::stoi (this->transactionId.substr(4));
    if(numericId >= this->countId) countId += 1;
}

Transaction::Transaction(const std::string& bookId, const std::string& userId, const TransactionType type)
    : bookId{bookId}, userId{userId}, type{type}
{
    this->transactionId = "TXN" + std::to_string(countId);
}


std::string Transaction::get_transactionId() const { return transactionId; }

std::string Transaction::get_bookId() const { return bookId; }

std::string Transaction::get_userId() const { return userId; }

TransactionType Transaction::get_type() const { return type; }

void Transaction::id_incrementor() { countId++; }
