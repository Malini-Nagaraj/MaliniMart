#pragma once

#include <drogon/drogon.h>
#include "../service/CartService.h"

class CartController
{
private:
    CartService service;

public:

    CartController(drogon::orm::DbClientPtr db)
        : service(db)
    {
    }


    // =========================
    // ADD TO CART
    // =========================

    void addToCart(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        auto json =
            req->getJsonObject();

        if (!json ||
            !json->isMember("userId") ||
            !json->isMember("productId") ||
            !json->isMember("quantity"))
        {
            Json::Value error;

            error["message"] =
                "userId, productId and quantity are required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        int userId =
            (*json)["userId"].asInt();

        int productId =
            (*json)["productId"].asInt();

        int quantity =
            (*json)["quantity"].asInt();

        if (quantity <= 0)
        {
            Json::Value error;

            error["message"] =
                "Quantity must be greater than 0";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        // CartService::addToCart has no callback
        service.addToCart(
            userId,
            productId,
            quantity
        );

        Json::Value result;

        result["message"] =
            "Product added to cart successfully";

        auto response =
            drogon::HttpResponse::newHttpJsonResponse(result);

        callback(response);
    }


    // =========================
    // GET CART
    // =========================

    void getCart(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        std::string userIdString =
            req->getParameter("userId");

        if (userIdString.empty())
        {
            Json::Value error;

            error["message"] =
                "userId is required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        int userId;

        try
        {
            userId =
                std::stoi(userIdString);
        }
        catch (...)
        {
            Json::Value error;

            error["message"] =
                "Invalid userId";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        service.getCart(
            userId,

            [callback](const drogon::orm::Result& result)
            {
                Json::Value cart;

                Json::Value items(
                    Json::arrayValue);

                double grandTotal = 0.0;

                for (const auto& row : result)
                {
                    Json::Value item;

                    item["id"] =
                        row["id"].as<int>();

                    item["productId"] =
                        row["product_id"].as<int>();

                    item["name"] =
                        row["name"].as<std::string>();

                    item["price"] =
                        row["price"].as<double>();

                    item["quantity"] =
                        row["quantity"].as<int>();

                    item["total"] =
                        row["total"].as<double>();

                    grandTotal +=
                        row["total"].as<double>();

                    items.append(item);
                }

                cart["items"] =
                    items;

                cart["grandTotal"] =
                    grandTotal;

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        cart);

                callback(response);
            });
    }


    // =========================
    // UPDATE CART
    // =========================

    void updateCart(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        std::string cartIdString =
            req->getParameter("cartId");

        if (cartIdString.empty())
        {
            Json::Value error;

            error["message"] =
                "cartId is required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        int cartId;

        try
        {
            cartId =
                std::stoi(cartIdString);
        }
        catch (...)
        {
            Json::Value error;

            error["message"] =
                "Invalid cartId";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        auto json =
            req->getJsonObject();

        if (!json ||
            !json->isMember("quantity"))
        {
            Json::Value error;

            error["message"] =
                "quantity is required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        int quantity =
            (*json)["quantity"].asInt();

        if (quantity <= 0)
        {
            Json::Value error;

            error["message"] =
                "Quantity must be greater than 0";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        service.updateCart(
            cartId,
            quantity,

            [callback](bool success)
            {
                Json::Value result;

                if (success)
                {
                    result["message"] =
                        "Cart updated successfully";

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(
                            result);

                    callback(response);
                }
                else
                {
                    result["message"] =
                        "Failed to update cart";

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
    // REMOVE FROM CART
    // =========================

    void removeFromCart(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        std::string cartIdString =
            req->getParameter("cartId");

        if (cartIdString.empty())
        {
            Json::Value error;

            error["message"] =
                "cartId is required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        int cartId;

        try
        {
            cartId =
                std::stoi(cartIdString);
        }
        catch (...)
        {
            Json::Value error;

            error["message"] =
                "Invalid cartId";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        service.removeFromCart(
            cartId,

            [callback](bool success)
            {
                Json::Value result;

                if (success)
                {
                    result["message"] =
                        "Product removed from cart";

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(
                            result);

                    callback(response);
                }
                else
                {
                    result["message"] =
                        "Failed to remove product from cart";

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