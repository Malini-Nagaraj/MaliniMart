

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <exception>

#include <drogon/drogon.h>
#include "../repository/UserRepository.h"

class UserController
{
private:
    UserRepository repository;

public:
    explicit UserController(drogon::orm::DbClientPtr db)
        : repository(db)
    {
    }

    // ---------------- REGISTER USER ----------------
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
            response->setBody("Name, email and password are required");
            callback(response);
            return;
        }

        User user;
        user.name = (*json)["name"].asString();
        user.email = (*json)["email"].asString();
        user.password = (*json)["password"].asString();

        repository.addUser(user);

        auto response = drogon::HttpResponse::newHttpResponse();
        response->setBody("User registration submitted.");
        callback(response);
    }

    // ---------------- LOGIN USER ----------------
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

        const std::string email = (*json)["email"].asString();
        const std::string password = (*json)["password"].asString();

        auto db = repository.getDbClient();

        auto cb = std::make_shared<
            std::function<void(const drogon::HttpResponsePtr&)>>(
                std::move(callback));

        db->execSqlAsync(
            "SELECT id, name, role FROM users "
            "WHERE email = $1 AND password = $2",

            [cb, req](const drogon::orm::Result& result)
            {
                if (result.empty())
                {
                    auto response = drogon::HttpResponse::newHttpResponse();
                    response->setStatusCode(drogon::k401Unauthorized);
                    response->setBody("Invalid email or password");
                    (*cb)(response);
                    return;
                }

                try
                {
                    const int userId = result[0]["id"].as<int>();
                    const std::string name =
                        result[0]["name"].as<std::string>();
                    const std::string role =
                        result[0]["role"].as<std::string>();

                    auto session = req->getSession();
                    session->insert("user_id", userId);
                    session->insert("user_role", role);
                    session->changeSessionIdToClient();

                    auto response = drogon::HttpResponse::newHttpResponse();
                    response->setBody(
                        "Login successful! Welcome " + name);
                    (*cb)(response);
                }
                catch (const std::exception&)
                {
                    auto response = drogon::HttpResponse::newHttpResponse();
                    response->setStatusCode(
                        drogon::k500InternalServerError);
                    response->setBody("Unable to create login session");
                    (*cb)(response);
                }
            },

            [cb](const drogon::orm::DrogonDbException&)
            {
                auto response = drogon::HttpResponse::newHttpResponse();
                response->setStatusCode(
                    drogon::k500InternalServerError);
                response->setBody("Database error");
                (*cb)(response);
            },

            email,
            password
        );
    }

    // ---------------- CHANGE PASSWORD ----------------
    void changePassword(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        auto json = req->getJsonObject();

        if (!json ||
            !json->isMember("email") ||
            !json->isMember("oldPassword") ||
            !json->isMember("newPassword"))
        {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k400BadRequest);
            response->setBody(
                "Email, oldPassword and newPassword are required");
            callback(response);
            return;
        }

        const std::string email = (*json)["email"].asString();
        const std::string oldPassword = (*json)["oldPassword"].asString();
        const std::string newPassword = (*json)["newPassword"].asString();

        if (newPassword.length() < 8)
        {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k400BadRequest);
            response->setBody(
                "New password must be at least 8 characters");
            callback(response);
            return;
        }

        auto db = repository.getDbClient();

        auto cb = std::make_shared<
            std::function<void(const drogon::HttpResponsePtr&)>>(
                std::move(callback));

        db->execSqlAsync(
            "UPDATE users SET password = $1 "
            "WHERE email = $2 AND password = $3",

            [cb](const drogon::orm::Result& result)
            {
                auto response = drogon::HttpResponse::newHttpResponse();

                if (result.affectedRows() == 0)
                {
                    response->setStatusCode(drogon::k401Unauthorized);
                    response->setBody(
                        "Invalid email or old password");
                }
                else
                {
                    response->setBody(
                        "Password changed successfully");
                }

                (*cb)(response);
            },

            [cb](const drogon::orm::DrogonDbException&)
            {
                auto response = drogon::HttpResponse::newHttpResponse();
                response->setStatusCode(
                    drogon::k500InternalServerError);
                response->setBody("Database error");
                (*cb)(response);
            },

            newPassword,
            email,
            oldPassword
        );
    }
};
