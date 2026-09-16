#pragma once

#include <drogon/drogon.h>
#include "../repository/ProductRepository.h"

class ProductController
{
private:
    ProductRepository repository;

public:

    ProductController(drogon::orm::DbClientPtr db)
        : repository(db)
    {
    }

    void addProduct(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        auto json = req->getJsonObject();

        if (!json ||
            !json->isMember("name") ||
            !json->isMember("price") ||
            !json->isMember("description"))
        {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k400BadRequest);
            response->setBody(
                "Name, price and description are required");
            callback(response);
            return;
        }

        Product product;
        product.name = (*json)["name"].asString();
        product.price = (*json)["price"].asDouble();
        product.description = (*json)["description"].asString();

        repository.addProduct(product);

        auto response = drogon::HttpResponse::newHttpResponse();
        response->setBody("Product added successfully!");
        callback(response);
    }

    void getAllProducts(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        repository.getAllProducts(
            [callback](const drogon::orm::Result& result)
            {
                Json::Value products(Json::arrayValue);

                for (const auto& row : result)
                {
                    Json::Value product;

                    product["id"] = row["id"].as<int>();
                    product["name"] = row["name"].as<std::string>();
                    product["price"] = row["price"].as<double>();
                    product["description"] =
                        row["description"].as<std::string>();

                    products.append(product);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(products);

                callback(response);
            });
    }
};