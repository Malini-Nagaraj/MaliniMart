#pragma once

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>

class Database
{
public:
    static drogon::orm::DbClientPtr getClient();
};