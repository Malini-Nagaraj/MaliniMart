
#pragma once



#include <iostream>

#include <drogon/drogon.h>
#include <functional>
#include <memory>
#include <string>

class AdminController
{
private:
    drogon::orm::DbClientPtr dbClient;

public:
    explicit AdminController(
        const drogon::orm::DbClientPtr& db)
        : dbClient(db)
    {
    }

    void getUsers(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        // Check login session
        auto session = req->getSession();
        auto userId = session->getOptional<int>("user_id");


        std::cout << "ADMIN SESSION USER ID: "
          << (userId ? std::to_string(*userId) : "MISSING")
          << std::endl;

        if (!userId)
        {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k401Unauthorized);
            response->setBody("Please log in first.");
            callback(response);
            return;
        }

        auto cb = std::make_shared<
            std::function<void(const drogon::HttpResponsePtr&)>>(
                std::move(callback));

        // Verify the logged-in user's role in the database
        dbClient->execSqlAsync(
            "SELECT role FROM users WHERE id = $1",

            [this, cb](const drogon::orm::Result& result)
            {
                if (result.empty() ||
                    result[0]["role"].as<std::string>() != "Admin")
                {
                    auto response = drogon::HttpResponse::newHttpResponse();
                    response->setStatusCode(drogon::k403Forbidden);
                    response->setBody("Admin access only.");
                    (*cb)(response);
                    return;
                }

                // Only an Admin can retrieve the user list
                dbClient->execSqlAsync(
                    "SELECT id, name, email, role "
                    "FROM users ORDER BY id",

                    [cb](const drogon::orm::Result& usersResult)
                    {
                        Json::Value users(Json::arrayValue);

                        for (const auto& row : usersResult)
                        {
                            Json::Value user;
                            user["id"] = row["id"].as<int>();
                            user["name"] = row["name"].as<std::string>();
                            user["email"] = row["email"].as<std::string>();
                            user["role"] = row["role"].as<std::string>();
                            users.append(user);
                        }

                        (*cb)(
                            drogon::HttpResponse::newHttpJsonResponse(users));
                    },

                    [cb](const drogon::orm::DrogonDbException&)
                    {
                        auto response =
                            drogon::HttpResponse::newHttpResponse();
                        response->setStatusCode(
                            drogon::k500InternalServerError);
                        response->setBody("Unable to load users.");
                        (*cb)(response);
                    });
            },

            [cb](const drogon::orm::DrogonDbException&)
            {
                auto response = drogon::HttpResponse::newHttpResponse();
                response->setStatusCode(drogon::k500InternalServerError);
                response->setBody("Unable to verify Admin permissions.");
                (*cb)(response);
            },

            *userId);
    }
};