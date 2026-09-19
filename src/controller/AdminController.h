#pragma once

#include <drogon/drogon.h>
#include <string>
#include "../service/AdminService.h"

class AdminController
{
private:
    AdminService service;

public:

    AdminController(drogon::orm::DbClientPtr db)
        : service(db)
    {
    }

    // ==========================================
    // GET ALL USERS
    // ==========================================
    void getAllUsers(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        service.getAllUsers(
            [callback](const drogon::orm::Result& result)
            {
                Json::Value users(Json::arrayValue);

                for (const auto& row : result)
                {
                    Json::Value user;

                    user["id"] =
                        row["id"].as<int>();

                    user["name"] =
                        row["name"].as<std::string>();

                    user["email"] =
                        row["email"].as<std::string>();

                    user["role"] =
                        row["role"].as<std::string>();

                    users.append(user);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(users);

                callback(response);
            });
    }

    // ==========================================
    // GET ALL ORDERS
    // ==========================================
    void getAllOrders(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        service.getAllOrders(
            [callback](const drogon::orm::Result& result)
            {
                Json::Value orders(Json::arrayValue);

                for (const auto& row : result)
                {
                    Json::Value order;

                    order["orderId"] =
                        row["order_id"].as<int>();

                    order["userId"] =
                        row["user_id"].as<int>();

                    order["userName"] =
                        row["user_name"].as<std::string>();

                    order["email"] =
                        row["email"].as<std::string>();

                    order["totalAmount"] =
                        row["total_amount"].as<double>();

                    order["status"] =
                        row["status"].as<std::string>();

                    order["createdAt"] =
                        row["created_at"].as<std::string>();

                    orders.append(order);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(orders);

                callback(response);
            });
    }

    // ==========================================
    // DELETE PRODUCT
    // ==========================================
    void deleteProduct(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        auto productIdString =
            req->getParameter("productId");

        if (productIdString.empty())
        {
            Json::Value errorJson;

            errorJson["message"] =
                "productId is required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(
                    errorJson);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        int productId;

        try
        {
            productId = std::stoi(productIdString);
        }
        catch (...)
        {
            Json::Value errorJson;

            errorJson["message"] =
                "Invalid productId";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(
                    errorJson);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        service.deleteProduct(
            productId,
            [callback](bool success)
            {
                Json::Value responseJson;

                if (success)
                {
                    responseJson["message"] =
                        "Product deleted successfully";

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(
                            responseJson);

                    callback(response);
                }
                else
                {
                    responseJson["message"] =
                        "Failed to delete product";

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(
                            responseJson);

                    response->setStatusCode(
                        drogon::k500InternalServerError);

                    callback(response);
                }
            });
    }
};