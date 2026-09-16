#include "UserService.h"
#include <iostream>

void UserService::registerUser(const User& user)
{
    std::cout << "User Registered Successfully!" << std::endl;
    std::cout << "Name: " << user.name << std::endl;
    std::cout << "Email: " << user.email << std::endl;
}