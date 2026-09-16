#pragma once

#include <drogon/drogon.h>
#include "../model/User.h"

class UserRepository
{
private:
    drogon::orm::DbClientPtr dbClient;

public:

    UserRepository(drogon::orm::DbClientPtr client)
    {
        dbClient = client;
    }

    drogon::orm::DbClientPtr getDbClient()
    {
        return dbClient;
    }

    void addUser(const User& user)
    {
        dbClient->execSqlAsync(
            "INSERT INTO users (name, email, password) "
            "VALUES ($1, $2, $3)",

            [](const drogon::orm::Result& result)
            {
                std::cout
                    << "User registered successfully!"
                    << std::endl;
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout
                    << "Database error: "
                    << e.base().what()
                    << std::endl;
            },

            user.name,
            user.email,
            user.password
        );
    }
};