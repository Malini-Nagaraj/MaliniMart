#pragma once

#include <drogon/drogon.h>
#include "../repository/AdminRepository.h"

class AdminService
{
private:
    AdminRepository repository;

public:

    AdminService(drogon::orm::DbClientPtr db)
        : repository(db)
    {
    }

    // Get all users
    void getAllUsers(
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        repository.getAllUsers(std::move(callback));
    }

    // Get all orders
    void getAllOrders(
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        repository.getAllOrders(std::move(callback));
    }

    // Delete product
    void deleteProduct(
        int productId,
        std::function<void(bool)>&& callback)
    {
        repository.deleteProduct(
            productId,
            std::move(callback)
        );
    }
};