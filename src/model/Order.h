#pragma once

#include <string>

struct Order
{
    int id;
    int userId;
    double totalAmount;
    std::string status;
    std::string createdAt;
};