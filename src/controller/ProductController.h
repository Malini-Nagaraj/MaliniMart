#pragma once

#include <drogon/drogon.h>
#include "../service/ProductService.h"

class ProductController
{
private:
    ProductService service;

public:

    ProductController(drogon::orm::DbClientPtr db)
        : service(db)
    {
    }

    // =========================
    // ADD PRODUCT
    // =========================

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
            Json::Value error;
            error["message"] =
                "name, price and description are required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(drogon::k400BadRequest);

            callback(response);
            return;
        }

        Product product;

        product.name =
            (*json)["name"].asString();

        product.price =
            (*json)["price"].asDouble();

        product.description =
            (*json)["description"].asString();

        // Image is optional
        if (json->isMember("image"))
        {
            product.image =
                (*json)["image"].asString();
        }

        service.addProduct(product);

        Json::Value result;
        result["message"] =
            "Product added successfully";

        auto response =
            drogon::HttpResponse::newHttpJsonResponse(result);

        callback(response);
    }


    // =========================
    // GET ALL PRODUCTS
    // =========================

    void getAllProducts(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        service.getAllProducts(
            [callback](const drogon::orm::Result& result)
            {
                Json::Value products(Json::arrayValue);

                for (const auto& row : result)
                {
                    Json::Value product;

                    product["id"] =
                        row["id"].as<int>();

                    product["name"] =
                        row["name"].as<std::string>();

                    product["price"] =
                        row["price"].as<double>();

                    product["description"] =
                        row["description"].as<std::string>();

                    product["image"] =
                        row["image"].as<std::string>();

                    products.append(product);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        products);

                callback(response);
            });
    }


    // =========================
    // SEARCH PRODUCTS
    // =========================

    void searchProducts(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        std::string keyword =
            req->getParameter("keyword");

        if (keyword.empty())
        {
            Json::Value error;
            error["message"] =
                "keyword is required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        service.searchProducts(
            keyword,
            [callback](const drogon::orm::Result& result)
            {
                Json::Value products(Json::arrayValue);

                for (const auto& row : result)
                {
                    Json::Value product;

                    product["id"] =
                        row["id"].as<int>();

                    product["name"] =
                        row["name"].as<std::string>();

                    product["price"] =
                        row["price"].as<double>();

                    product["description"] =
                        row["description"].as<std::string>();

                    product["image"] =
                        row["image"].as<std::string>();

                    products.append(product);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        products);

                callback(response);
            });
    }


    // =========================
    // UPDATE PRODUCT
    // =========================

    void updateProduct(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        std::string productIdString =
            req->getParameter("productId");

        if (productIdString.empty())
        {
            Json::Value error;
            error["message"] =
                "productId is required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(drogon::k400BadRequest);

            callback(response);
            return;
        }

        int productId;

        try
        {
            productId =
                std::stoi(productIdString);
        }
        catch (...)
        {
            Json::Value error;
            error["message"] =
                "Invalid productId";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(drogon::k400BadRequest);

            callback(response);
            return;
        }

        auto json =
            req->getJsonObject();

        if (!json ||
            !json->isMember("name") ||
            !json->isMember("price") ||
            !json->isMember("description"))
        {
            Json::Value error;
            error["message"] =
                "name, price and description are required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(drogon::k400BadRequest);

            callback(response);
            return;
        }

        Product product;

        product.id =
            productId;

        product.name =
            (*json)["name"].asString();

        product.price =
            (*json)["price"].asDouble();

        product.description =
            (*json)["description"].asString();

        // Image is optional
        if (json->isMember("image"))
        {
            product.image =
                (*json)["image"].asString();
        }

        service.updateProduct(
            productId,
            product,

            [callback](bool success)
            {
                Json::Value result;

                if (success)
                {
                    result["message"] =
                        "Product updated successfully";

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(
                            result);

                    callback(response);
                }
                else
                {
                    result["message"] =
                        "Failed to update product";

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(
                            result);

                    response->setStatusCode(
                        drogon::k500InternalServerError);

                    callback(response);
                }
            });
    }


    // =========================
    // DELETE PRODUCT
    // =========================

    void deleteProduct(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        std::string productIdString =
            req->getParameter("productId");

        if (productIdString.empty())
        {
            Json::Value error;
            error["message"] =
                "productId is required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(drogon::k400BadRequest);

            callback(response);
            return;
        }

        int productId;

        try
        {
            productId =
                std::stoi(productIdString);
        }
        catch (...)
        {
            Json::Value error;
            error["message"] =
                "Invalid productId";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(drogon::k400BadRequest);

            callback(response);
            return;
        }

        service.deleteProduct(
            productId,

            [callback](bool success)
            {
                Json::Value result;

                if (success)
                {
                    result["message"] =
                        "Product deleted successfully";

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(
                            result);

                    callback(response);
                }
                else
                {
                    result["message"] =
                        "Failed to delete product";

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(
                            result);

                    response->setStatusCode(
                        drogon::k500InternalServerError);

                    callback(response);
                }
            });
    }
};