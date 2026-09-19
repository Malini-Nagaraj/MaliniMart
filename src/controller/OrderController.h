#pragma once

#include <drogon/drogon.h>
#include "../service/OrderService.h"

class OrderController
{
private:
    OrderService service;

public:

    OrderController(drogon::orm::DbClientPtr db)
        : service(db)
    {
    }


    // ==========================================
    // COMPLETE CHECKOUT
    // ==========================================

    void checkout(
        const drogon::HttpRequestPtr& req,
        std::function<void(
            const drogon::HttpResponsePtr&)>&& callback)
    {
        auto json = req->getJsonObject();

        if (!json || !json->isMember("userId"))
        {
            auto response =
                drogon::HttpResponse::newHttpResponse();

            response->setStatusCode(
                drogon::k400BadRequest);

            response->setBody(
                "userId is required");

            callback(response);
            return;
        }

        int userId =
            (*json)["userId"].asInt();

        if (userId <= 0)
        {
            auto response =
                drogon::HttpResponse::newHttpResponse();

            response->setStatusCode(
                drogon::k400BadRequest);

            response->setBody(
                "Invalid userId");

            callback(response);
            return;
        }

        service.getCartForCheckout(
            userId,
            [this, userId, callback](
                const drogon::orm::Result& cart)
            {
                if (cart.empty())
                {
                    auto response =
                        drogon::HttpResponse::newHttpResponse();

                    response->setStatusCode(
                        drogon::k400BadRequest);

                    response->setBody(
                        "Cart is empty");

                    callback(response);
                    return;
                }

                double totalAmount = 0.0;

                for (const auto& row : cart)
                {
                    double price =
                        row["price"].as<double>();

                    int quantity =
                        row["quantity"].as<int>();

                    totalAmount +=
                        price * quantity;
                }

                service.createOrder(
                    userId,
                    totalAmount,
                    [this,
                     userId,
                     cart,
                     totalAmount,
                     callback](int orderId)
                    {
                        if (orderId <= 0)
                        {
                            auto response =
                                drogon::HttpResponse::
                                newHttpResponse();

                            response->setStatusCode(
                                drogon::
                                k500InternalServerError);

                            response->setBody(
                                "Failed to create order");

                            callback(response);
                            return;
                        }

                        auto addItems =
                            std::make_shared<
                                std::function<void(size_t)>>();

                        *addItems =
                            [this,
                             userId,
                             cart,
                             totalAmount,
                             orderId,
                             callback,
                             addItems](size_t index)
                            {
                                if (index >= cart.size())
                                {
                                    service.clearCart(
                                        userId,
                                        [orderId,
                                         totalAmount,
                                         callback]
                                        (bool success)
                                        {
                                            if (!success)
                                            {
                                                auto response =
                                                    drogon::
                                                    HttpResponse::
                                                    newHttpResponse();

                                                response->setStatusCode(
                                                    drogon::
                                                    k500InternalServerError);

                                                response->setBody(
                                                    "Order created but cart could not be cleared");

                                                callback(response);
                                                return;
                                            }

                                            Json::Value result;

                                            result["message"] =
                                                "Mock payment successful! Order confirmed.";

                                            result["orderId"] =
                                                orderId;

                                            result["totalAmount"] =
                                                totalAmount;

                                            result["status"] =
                                                "Confirmed";

                                            auto response =
                                                drogon::
                                                HttpResponse::
                                                newHttpJsonResponse(
                                                    result);

                                            callback(response);
                                        });

                                    return;
                                }

                                int productId =
                                    cart[index]
                                    ["product_id"]
                                    .as<int>();

                                int quantity =
                                    cart[index]
                                    ["quantity"]
                                    .as<int>();

                                double price =
                                    cart[index]
                                    ["price"]
                                    .as<double>();

                                service.addOrderItem(
                                    orderId,
                                    productId,
                                    quantity,
                                    price,
                                    [addItems,
                                     index](bool success)
                                    {
                                        if (!success)
                                        {
                                            return;
                                        }

                                        (*addItems)(
                                            index + 1);
                                    });
                            };

                        (*addItems)(0);
                    });
            });
    }


    // ==========================================
    // CREATE ORDER
    // ==========================================

    void createOrder(
        const drogon::HttpRequestPtr& req,
        std::function<void(
            const drogon::HttpResponsePtr&)>&& callback)
    {
        auto json = req->getJsonObject();

        if (!json ||
            !json->isMember("userId") ||
            !json->isMember("totalAmount"))
        {
            auto response =
                drogon::HttpResponse::newHttpResponse();

            response->setStatusCode(
                drogon::k400BadRequest);

            response->setBody(
                "userId and totalAmount are required");

            callback(response);
            return;
        }

        int userId =
            (*json)["userId"].asInt();

        double totalAmount =
            (*json)["totalAmount"].asDouble();

        if (userId <= 0 ||
            totalAmount <= 0)
        {
            auto response =
                drogon::HttpResponse::newHttpResponse();

            response->setStatusCode(
                drogon::k400BadRequest);

            response->setBody(
                "Invalid userId or totalAmount");

            callback(response);
            return;
        }

        service.createOrder(
            userId,
            totalAmount,
            [callback](int orderId)
            {
                if (orderId > 0)
                {
                    Json::Value result;

                    result["message"] =
                        "Order created successfully!";

                    result["orderId"] =
                        orderId;

                    auto response =
                        drogon::HttpResponse::
                        newHttpJsonResponse(result);

                    callback(response);
                }
                else
                {
                    auto response =
                        drogon::HttpResponse::
                        newHttpResponse();

                    response->setStatusCode(
                        drogon::
                        k500InternalServerError);

                    response->setBody(
                        "Failed to create order");

                    callback(response);
                }
            });
    }


    // ==========================================
    // BUYER ORDER HISTORY
    // ==========================================

    void getOrders(
        const drogon::HttpRequestPtr& req,
        std::function<void(
            const drogon::HttpResponsePtr&)>&& callback)
    {
        std::string userIdString =
            req->getParameter("userId");

        if (userIdString.empty())
        {
            auto response =
                drogon::HttpResponse::newHttpResponse();

            response->setStatusCode(
                drogon::k400BadRequest);

            response->setBody(
                "userId is required");

            callback(response);
            return;
        }

        int userId =
            std::stoi(userIdString);

        service.getOrders(
            userId,
            [callback](
                const drogon::orm::Result& result)
            {
                Json::Value orders(
                    Json::arrayValue);

                for (const auto& row : result)
                {
                    Json::Value order;

                    order["id"] =
                        row["id"].as<int>();

                    order["userId"] =
                        row["user_id"].as<int>();

                    order["totalAmount"] =
                        row["total_amount"]
                        .as<double>();

                    order["status"] =
                        row["status"]
                        .as<std::string>();

                    order["createdAt"] =
                        row["created_at"]
                        .as<std::string>();

                    orders.append(order);
                }

                auto response =
                    drogon::HttpResponse::
                    newHttpJsonResponse(
                        orders);

                callback(response);
            });
    }


    // ==========================================
    // SELLER INCOMING ORDERS
    // ==========================================

    void getSellerOrders(
        const drogon::HttpRequestPtr& req,
        std::function<void(
            const drogon::HttpResponsePtr&)>&& callback)
    {
        std::string sellerIdString =
            req->getParameter("sellerId");

        if (sellerIdString.empty())
        {
            auto response =
                drogon::HttpResponse::newHttpResponse();

            response->setStatusCode(
                drogon::k400BadRequest);

            response->setBody(
                "sellerId is required");

            callback(response);
            return;
        }

        int sellerId;

        try
        {
            sellerId =
                std::stoi(sellerIdString);
        }
        catch (...)
        {
            auto response =
                drogon::HttpResponse::newHttpResponse();

            response->setStatusCode(
                drogon::k400BadRequest);

            response->setBody(
                "Invalid sellerId");

            callback(response);
            return;
        }

        if (sellerId <= 0)
        {
            auto response =
                drogon::HttpResponse::newHttpResponse();

            response->setStatusCode(
                drogon::k400BadRequest);

            response->setBody(
                "Invalid sellerId");

            callback(response);
            return;
        }

        service.getSellerOrders(
            sellerId,
            [callback](
                const drogon::orm::Result& result)
            {
                Json::Value orders(
                    Json::arrayValue);

                for (const auto& row : result)
                {
                    Json::Value order;

                    order["orderId"] =
                        row["order_id"].as<int>();

                    order["userId"] =
                        row["user_id"].as<int>();

                    order["totalAmount"] =
                        row["total_amount"]
                        .as<double>();

                    order["status"] =
                        row["status"]
                        .as<std::string>();

                    order["createdAt"] =
                        row["created_at"]
                        .as<std::string>();

                    order["productId"] =
                        row["product_id"].as<int>();

                    order["productName"] =
                        row["product_name"]
                        .as<std::string>();

                    order["quantity"] =
                        row["quantity"].as<int>();

                    order["price"] =
                        row["price"].as<double>();

                    orders.append(order);
                }

                auto response =
                    drogon::HttpResponse::
                    newHttpJsonResponse(
                        orders);

                callback(response);
            });
    }
};