#pragma once

#include <drogon/drogon.h>
#include "../repository/CartRepository.h"

class CartService
{
private:
    CartRepository repository;

public:

    CartService(drogon::orm::DbClientPtr db)
        : repository(db)
    {
    }

    // Add product to cart
    void addToCart(
        int userId,
        int productId,
        int quantity)
    {
        repository.addToCart(
            userId,
            productId,
            quantity
        );
    }

    // Get cart
    void getCart(
        int userId,
        std::function<void(
            const drogon::orm::Result&)>&& callback)
    {
        repository.getCart(
            userId,
            std::move(callback)
        );
    }

    // Update cart quantity
    void updateCart(
        int cartId,
        int quantity,
        std::function<void(bool)>&& callback)
    {
        repository.updateCart(
            cartId,
            quantity,
            std::move(callback)
        );
    }

    // Remove product from cart
    void removeFromCart(
        int cartId,
        std::function<void(bool)>&& callback)
    {
        repository.removeFromCart(
            cartId,
            std::move(callback)
        );
    }
};