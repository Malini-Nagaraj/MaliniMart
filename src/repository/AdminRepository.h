#pragma once

#include <drogon/drogon.h>

class AdminRepository
{
private:
    drogon::orm::DbClientPtr dbClient;

public:

    AdminRepository(drogon::orm::DbClientPtr client)
        : dbClient(client)
    {
    }

    // Get all users
    void getAllUsers(
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        dbClient->execSqlAsync(
            "SELECT id, name, email, role "
            "FROM users "
            "ORDER BY id",

            [callback](const drogon::orm::Result& result)
            {
                callback(result);
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout << "Admin users database error: "
                          << e.base().what()
                          << std::endl;
            }
        );
    }

    // Get all orders
    void getAllOrders(
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        dbClient->execSqlAsync(
            "SELECT "
            "o.id AS order_id, "
            "o.user_id, "
            "u.name AS user_name, "
            "u.email, "
            "o.total_amount, "
            "o.status, "
            "o.created_at "
            "FROM orders o "
            "JOIN users u ON o.user_id = u.id "
            "ORDER BY o.id DESC",

            [callback](const drogon::orm::Result& result)
            {
                callback(result);
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout << "Admin orders database error: "
                          << e.base().what()
                          << std::endl;
            }
        );
    }

    // Delete product
    void deleteProduct(
        int productId,
        std::function<void(bool)>&& callback)
    {
        dbClient->execSqlAsync(
            "DELETE FROM products WHERE id = $1",

            [callback](const drogon::orm::Result& result)
            {
                callback(true);
            },

            [callback](const drogon::orm::DrogonDbException& e)
            {
                std::cout << "Admin delete product error: "
                          << e.base().what()
                          << std::endl;

                callback(false);
            },

            productId
        );
    }
};