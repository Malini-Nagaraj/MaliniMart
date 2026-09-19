#pragma once

#include <drogon/drogon.h>
#include "../repository/ReviewRepository.h"

class ReviewService
{
private:
    ReviewRepository repository;

public:

    ReviewService(drogon::orm::DbClientPtr db)
        : repository(db)
    {
    }

    void addReview(
        int userId,
        int productId,
        int rating,
        const std::string& comment,
        std::function<void(bool)>&& callback)
    {
        repository.addReview(
            userId,
            productId,
            rating,
            comment,
            std::move(callback)
        );
    }

    void getProductReviews(
        int productId,
        std::function<void(const drogon::orm::Result&)>&& callback)
    {
        repository.getProductReviews(
            productId,
            std::move(callback)
        );
    }
};