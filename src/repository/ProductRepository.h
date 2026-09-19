#pragma once

#include <drogon/drogon.h>
#include "../model/Product.h"

class ProductRepository
{
private:
    drogon::orm::DbClientPtr dbClient;

public:

    // Constructor
    ProductRepository(drogon::orm::DbClientPtr client)
    {
        dbClient = client;
    }


    // =========================
    // ADD PRODUCT
    // =========================

    void addProduct(const Product& product)
    {
        dbClient->execSqlAsync(
            "INSERT INTO products (name, price, description) "
            "VALUES ($1, $2, $3)",

            [](const drogon::orm::Result& result)
            {
                std::cout
                    << "Product added successfully!"
                    << std::endl;
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Database error: "
                    << e.base().what()
                    << std::endl;
            },

            product.name,
            product.price,
            product.description
        );
    }


    // =========================
    // GET ALL PRODUCTS
    // =========================

    void getAllProducts(
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        dbClient->execSqlAsync(
            "SELECT id, name, price, description "
            "FROM products "
            "ORDER BY id",

            [callback](const drogon::orm::Result& result)
            {
                callback(result);
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Database error: "
                    << e.base().what()
                    << std::endl;
            }
        );
    }


    // =========================
    // SEARCH PRODUCTS
    // =========================

    void searchProducts(
        const std::string& keyword,
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        dbClient->execSqlAsync(
            "SELECT id, name, price, description "
            "FROM products "
            "WHERE LOWER(name) LIKE LOWER($1) "
            "OR LOWER(description) LIKE LOWER($1) "
            "ORDER BY id",

            [callback](const drogon::orm::Result& result)
            {
                callback(result);
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Database error: "
                    << e.base().what()
                    << std::endl;
            },

            "%" + keyword + "%"
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
        dbClient->execSqlAsync(
            "UPDATE products "
            "SET name = $1, price = $2, description = $3 "
            "WHERE id = $4",

            [callback](const drogon::orm::Result& result)
            {
                callback(result.affectedRows() > 0);
            },

            [callback](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Database error: "
                    << e.base().what()
                    << std::endl;

                callback(false);
            },

            product.name,
            product.price,
            product.description,
            id
        );
    }


    // =========================
    // DELETE PRODUCT
    // =========================

    void deleteProduct(
        int id,
        std::function<void(bool)>&& callback)
    {
        dbClient->execSqlAsync(
            "DELETE FROM products WHERE id = $1",

            [callback](const drogon::orm::Result& result)
            {
                callback(result.affectedRows() > 0);
            },

            [callback](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Database error: "
                    << e.base().what()
                    << std::endl;

                callback(false);
            },

            id
        );
    }
};