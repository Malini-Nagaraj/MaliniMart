#include "Database.h"

drogon::orm::DbClientPtr Database::getClient()
{
    static auto client = drogon::orm::DbClient::newPgClient(
        "host=localhost port=5432 dbname=malinimart user=postgres",
        1,
        false
    );

    return client;
}