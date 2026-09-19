#pragma once

#include <drogon/drogon.h>
#include "../repository/ProductRepository.h"

class ProductService
{
private:
    ProductRepository repository;

public:

    // Constructor
    ProductService(drogon::orm::DbClientPtr db)
        : repository(db)
    {
    }


    // =========================
    // ADD PRODUCT
    // =========================

    void addProduct(const Product& product)
    {
        repository.addProduct(product);
    }


    // =========================
    // GET ALL PRODUCTS
    // =========================

    void getAllProducts(
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        repository.getAllProducts(
            std::move(callback)
        );
    }


    // =========================
    // SEARCH PRODUCTS
    // =========================

    void searchProducts(
        const std::string& keyword,
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        repository.searchProducts(
            keyword,
            std::move(callback)
        );
    }


    // =========================
    // UPDATE PRODUCT
    // =========================

    void updateProduct(
        int id,
        const Product& product,
        std::function<void(bool)>&& callback)
    {
        repository.updateProduct(
            id,
            product,
            std::move(callback)
        );
    }


    // =========================
    // DELETE PRODUCT
    // =========================

    void deleteProduct(
        int id,
        std::function<void(bool)>&& callback)
    {
        repository.deleteProduct(
            id,
            std::move(callback)
        );
    }
};