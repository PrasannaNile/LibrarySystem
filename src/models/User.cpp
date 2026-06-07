#include "models/User.h"


User::User(std::string _userId, std::string _name, std::string _email, UserRole _role) :
    userId{_userId}, name{_name}, email{_email}, role{_role}
{}

std::string User::get_userId() const { return userId; }
std::string User::get_name() const { return name; }
std::string User::get_email() const { return email; }
UserRole User::get_role() const { return role; }