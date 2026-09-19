#pragma once

#include <drogon/drogon.h>
#include "../model/CartItem.h"

class CartRepository
{
private:
    drogon::orm::DbClientPtr dbClient;

public:

    CartRepository(drogon::orm::DbClientPtr client)
    {
        dbClient = client;
    }

    // Add product to cart
    void addToCart(
        int userId,
        int productId,
        int quantity)
    {
        dbClient->execSqlAsync(
            "INSERT INTO cart_items "
            "(user_id, product_id, quantity) "
            "VALUES ($1, $2, $3)",
            [](const drogon::orm::Result& result)
            {
                std::cout
                    << "Product added to cart!"
                    << std::endl;
            },
            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Cart database error: "
                    << e.base().what()
                    << std::endl;
            },
            userId,
            productId,
            quantity
        );
    }

    // Get user's cart
    void getCart(
        int userId,
        std::function<void(
            const drogon::orm::Result&)>&& callback)
    {
        dbClient->execSqlAsync(
            "SELECT "
            "c.id, "
            "c.product_id, "
            "p.name, "
            "p.price, "
            "c.quantity, "
            "(p.price * c.quantity) AS total "
            "FROM cart_items c "
            "JOIN products p "
            "ON c.product_id = p.id "
            "WHERE c.user_id = $1 "
            "ORDER BY c.id",
            [callback](
                const drogon::orm::Result& result)
            {
                callback(result);
            },
            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Cart database error: "
                    << e.base().what()
                    << std::endl;
            },
            userId
        );
    }

    // Update cart quantity
    void updateCart(
        int cartId,
        int quantity,
        std::function<void(bool)>&& callback)
    {
        dbClient->execSqlAsync(
            "UPDATE cart_items "
            "SET quantity = $1 "
            "WHERE id = $2",
            [callback](
                const drogon::orm::Result& result)
            {
                callback(result.affectedRows() > 0);
            },
            [callback](
                const drogon::orm::DrogonDbException& e)
            {
                callback(false);
            },
            quantity,
            cartId
        );
    }

    // Remove product from cart
    void removeFromCart(
        int cartId,
        std::function<void(bool)>&& callback)
    {
        dbClient->execSqlAsync(
            "DELETE FROM cart_items "
            "WHERE id = $1",
            [callback](
                const drogon::orm::Result& result)
            {
                callback(result.affectedRows() > 0);
            },
            [callback](
                const drogon::orm::DrogonDbException& e)
            {
                callback(false);
            },
            cartId
        );
    }
};