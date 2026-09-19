#pragma once

#include <drogon/drogon.h>

class ReviewRepository
{
private:
    drogon::orm::DbClientPtr dbClient;

public:

    ReviewRepository(drogon::orm::DbClientPtr client)
        : dbClient(client)
    {
    }

    // Add a review
    void addReview(
        int userId,
        int productId,
        int rating,
        const std::string& comment,
        std::function<void(bool)>&& callback)
    {
        dbClient->execSqlAsync(
            "INSERT INTO reviews "
            "(user_id, product_id, rating, comment) "
            "VALUES ($1, $2, $3, $4)",

            [callback](const drogon::orm::Result& result)
            {
                callback(true);
            },

            [callback](const drogon::orm::DrogonDbException& e)
            {
                std::cout << "Review database error: "
                          << e.base().what()
                          << std::endl;

                callback(false);
            },

            userId,
            productId,
            rating,
            comment
        );
    }

    // Get reviews for a product
    void getProductReviews(
        int productId,
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        dbClient->execSqlAsync(
            "SELECT "
            "r.id, "
            "r.user_id, "
            "u.name AS user_name, "
            "r.product_id, "
            "r.rating, "
            "r.comment, "
            "r.created_at "
            "FROM reviews r "
            "JOIN users u ON r.user_id = u.id "
            "WHERE r.product_id = $1 "
            "ORDER BY r.id DESC",

            [callback](const drogon::orm::Result& result)
            {
                callback(result);
            },

            [](const drogon::orm::DrogonDbException& e)
            {
                std::cout << "Get reviews database error: "
                          << e.base().what()
                          << std::endl;
            },

            productId
        );
    }
};