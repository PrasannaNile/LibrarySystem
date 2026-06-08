#include "models/Transaction.h"


Transaction::Transaction(const std::string& transactionId, const std::string& bookId, const std::string& userId, const TransactionType type)
    : transactionId{transactionId}, bookId{bookId}, userId{userId}, type{type}
{}


std::string Transaction::get_transactionId() const { return transactionId; }

std::string Transaction::get_bookId() const { return bookId; }

std::string Transaction::get_userId() const { return userId; }

TransactionType Transaction::get_type() const { return type; }


