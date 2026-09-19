#pragma once

#include <drogon/drogon.h>
#include "../model/Order.h"
#include "../model/OrderItem.h"

class OrderRepository
{
private:
    drogon::orm::DbClientPtr dbClient;

public:

    OrderRepository(drogon::orm::DbClientPtr client)
    {
        dbClient = client;
    }


    // ==========================================
    // CREATE ORDER
    // ==========================================

    void createOrder(
        int userId,
        double totalAmount,
        std::function<void(int)>&& callback)
    {
        dbClient->execSqlAsync(
            "INSERT INTO orders "
            "(user_id, total_amount, status) "
            "VALUES ($1, $2, 'Confirmed') "
            "RETURNING id",

            [callback](const drogon::orm::Result& result)
            {
                if (result.size() > 0)
                {
                    int orderId =
                        result[0]["id"].as<int>();

                    callback(orderId);
                }
                else
                {
                    callback(-1);
                }
            },

            [callback](
                const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Order database error: "
                    << e.base().what()
                    << std::endl;

                callback(-1);
            },

            userId,
            totalAmount
        );
    }


    // ==========================================
    // ADD ITEM TO ORDER
    // ==========================================

    void addOrderItem(
        int orderId,
        int productId,
        int quantity,
        double price,
        std::function<void(bool)>&& callback)
    {
        dbClient->execSqlAsync(
            "INSERT INTO order_items "
            "(order_id, product_id, quantity, price) "
            "VALUES ($1, $2, $3, $4)",

            [callback](const drogon::orm::Result& result)
            {
                callback(true);
            },

            [callback](
                const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Order item database error: "
                    << e.base().what()
                    << std::endl;

                callback(false);
            },

            orderId,
            productId,
            quantity,
            price
        );
    }


    // ==========================================
    // GET CART FOR CHECKOUT
    // ==========================================

    void getCartForCheckout(
        int userId,
        std::function<void(
            const drogon::orm::Result&)>&& callback)
    {
        dbClient->execSqlAsync(
            "SELECT product_id, quantity, price "
            "FROM cart_items "
            "JOIN products "
            "ON cart_items.product_id = products.id "
            "WHERE cart_items.user_id = $1",

            [callback](
                const drogon::orm::Result& result)
            {
                callback(result);
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Checkout cart error: "
                    << e.base().what()
                    << std::endl;
            },

            userId
        );
    }


    // ==========================================
    // CLEAR CART
    // ==========================================

    void clearCart(
        int userId,
        std::function<void(bool)>&& callback)
    {
        dbClient->execSqlAsync(
            "DELETE FROM cart_items "
            "WHERE user_id = $1",

            [callback](
                const drogon::orm::Result& result)
            {
                callback(true);
            },

            [callback](
                const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Clear cart error: "
                    << e.base().what()
                    << std::endl;

                callback(false);
            },

            userId
        );
    }


    // ==========================================
    // GET USER ORDER HISTORY
    // ==========================================

    void getOrders(
        int userId,
        std::function<void(
            const drogon::orm::Result&)>&& callback)
    {
        dbClient->execSqlAsync(
            "SELECT id, user_id, total_amount, "
            "status, created_at "
            "FROM orders "
            "WHERE user_id = $1 "
            "ORDER BY id DESC",

            [callback](
                const drogon::orm::Result& result)
            {
                callback(result);
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Order history database error: "
                    << e.base().what()
                    << std::endl;
            },

            userId
        );
    }


    // ==========================================
    // GET SELLER ORDERS
    // ==========================================

    void getSellerOrders(
        int sellerId,
        std::function<void(
            const drogon::orm::Result&)>&& callback)
    {
        dbClient->execSqlAsync(
            "SELECT "
            "o.id AS order_id, "
            "o.user_id, "
            "o.total_amount, "
            "o.status, "
            "o.created_at, "
            "oi.product_id, "
            "p.name AS product_name, "
            "oi.quantity, "
            "oi.price "
            "FROM orders o "
            "JOIN order_items oi "
            "ON o.id = oi.order_id "
            "JOIN products p "
            "ON oi.product_id = p.id "
            "WHERE p.seller_id = $1 "
            "ORDER BY o.id DESC",

            [callback](
                const drogon::orm::Result& result)
            {
                callback(result);
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Seller orders database error: "
                    << e.base().what()
                    << std::endl;
            },

            sellerId
        );
    }
};