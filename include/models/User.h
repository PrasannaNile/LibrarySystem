# pragma once

#include <string>

enum class UserRole {STUDENT, ADMIN};


class User {
private:
    std::string userId;
    std::string name;
    std::string email;
    UserRole role;

public:
    User(std::string _userId, std::string _name, std::string _email, UserRole _role);
    std::string get_userId() const;
    std::string get_name() const;
    std::string get_email() const;
    UserRole get_role() const;

};