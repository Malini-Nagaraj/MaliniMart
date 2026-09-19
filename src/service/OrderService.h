#pragma once

#include <drogon/drogon.h>
#include "../repository/OrderRepository.h"

class OrderService
{
private:
    OrderRepository repository;

public:

    OrderService(drogon::orm::DbClientPtr db)
        : repository(db)
    {
    }


    // ==========================================
    // CREATE ORDER
    // ==========================================

    void createOrder(
        int userId,
        double totalAmount,
        std::function<void(int)>&& callback)
    {
        repository.createOrder(
            userId,
            totalAmount,
            std::move(callback)
        );
    }


    // ==========================================
    // ADD ORDER ITEM
    // ==========================================

    void addOrderItem(
        int orderId,
        int productId,
        int quantity,
        double price,
        std::function<void(bool)>&& callback)
    {
        repository.addOrderItem(
            orderId,
            productId,
            quantity,
            price,
            std::move(callback)
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
        repository.getCartForCheckout(
            userId,
            std::move(callback)
        );
    }


    // ==========================================
    // CLEAR CART
    // ==========================================

    void clearCart(
        int userId,
        std::function<void(bool)>&& callback)
    {
        repository.clearCart(
            userId,
            std::move(callback)
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
        repository.getOrders(
            userId,
            std::move(callback)
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
        repository.getSellerOrders(
            sellerId,
            std::move(callback)
        );
    }
};