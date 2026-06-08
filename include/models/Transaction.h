# pragma once

#include <string>


class Transaction {
private:
    std::string transactionId;
    std::string bookId;
    std::string userId;
public:
    Transaction() = default;
};