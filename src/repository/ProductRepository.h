#pragma once

#include <drogon/drogon.h>
#include "../model/Product.h"

class ProductRepository
{
private:
    drogon::orm::DbClientPtr dbClient;

public:

    ProductRepository(drogon::orm::DbClientPtr client)
    {
        dbClient = client;
    }

    void addProduct(const Product& product)
    {
        dbClient->execSqlAsync(
            "INSERT INTO products (name, price, description) "
            "VALUES ($1, $2, $3)",

            [](const drogon::orm::Result& result)
            {
                std::cout << "Product added successfully!"
                          << std::endl;
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout << "Database error: "
                          << e.base().what()
                          << std::endl;
            },

            product.name,
            product.price,
            product.description
        );
    }

    void getAllProducts(
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        dbClient->execSqlAsync(
            "SELECT id, name, price, description "
            "FROM products",

            [callback](const drogon::orm::Result& result)
            {
                callback(result);
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout << "Database error: "
                          << e.base().what()
                          << std::endl;
            }
        );
    }
};