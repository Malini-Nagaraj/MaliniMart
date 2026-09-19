#pragma once

#include <string>

struct Review
{
    int id;
    int userId;
    int productId;
    int rating;
    std::string comment;
    std::string createdAt;
};