#pragma once

#include <drogon/drogon.h>
#include "../repository/UserRepository.h"

class UserController
{
private:
    UserRepository repository;

public:
    UserController(drogon::orm::DbClientPtr db)
        : repository(db)
    {
    }

    // Register
    void registerUser(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        auto json = req->getJsonObject();

        if (!json ||
            !json->isMember("name") ||
            !json->isMember("email") ||
            !json->isMember("password"))
        {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k400BadRequest);
            response->setBody(
                "Name, email and password are required");
            callback(response);
            return;
        }

        User user;
        user.name = (*json)["name"].asString();
        user.email = (*json)["email"].asString();
        user.password = (*json)["password"].asString();

        repository.addUser(user);

        auto response = drogon::HttpResponse::newHttpResponse();
        response->setBody("User registered successfully!");
        callback(response);
    }

    // Login
    void loginUser(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        auto json = req->getJsonObject();

        if (!json ||
            !json->isMember("email") ||
            !json->isMember("password"))
        {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k400BadRequest);
            response->setBody("Email and password are required");
            callback(response);
            return;
        }

        std::string email = (*json)["email"].asString();
        std::string password = (*json)["password"].asString();

        auto db = repository.getDbClient();

        db->execSqlAsync(
            "SELECT id, name FROM users "
            "WHERE email = $1 AND password = $2",
            
            [callback](const drogon::orm::Result& result)
            {
                auto response =
                    drogon::HttpResponse::newHttpResponse();

                if (result.size() > 0)
                {
                    response->setBody(
                        "Login successful! Welcome " +
                        result[0]["name"].as<std::string>());
                }
                else
                {
                    response->setStatusCode(
                        drogon::k401Unauthorized);

                    response->setBody(
                        "Invalid email or password");
                }

                callback(response);
            },

            [callback](const drogon::orm::DrogonDbException& e)
            {
                auto response =
                    drogon::HttpResponse::newHttpResponse();

                response->setStatusCode(
                    drogon::k500InternalServerError);

                response->setBody(
                    "Database error");

                callback(response);
            },

            email,
            password
        );
    }
};