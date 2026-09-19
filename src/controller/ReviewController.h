#pragma once

#include <drogon/drogon.h>
#include "../service/ReviewService.h"

class ReviewController
{
private:
    ReviewService service;

public:

    ReviewController(drogon::orm::DbClientPtr db)
        : service(db)
    {
    }

    // Add review
    void addReview(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        auto json = req->getJsonObject();

        if (!json ||
            !json->isMember("userId") ||
            !json->isMember("productId") ||
            !json->isMember("rating"))
        {
            Json::Value error;
            error["message"] =
                "userId, productId and rating are required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        int userId = (*json)["userId"].asInt();
        int productId = (*json)["productId"].asInt();
        int rating = (*json)["rating"].asInt();

        std::string comment = "";

        if (json->isMember("comment"))
        {
            comment = (*json)["comment"].asString();
        }

        // Rating must be between 1 and 5
        if (rating < 1 || rating > 5)
        {
            Json::Value error;
            error["message"] =
                "Rating must be between 1 and 5";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        service.addReview(
            userId,
            productId,
            rating,
            comment,

            [callback](bool success)
            {
                Json::Value result;

                if (success)
                {
                    result["message"] =
                        "Review added successfully";

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(
                            result);

                    callback(response);
                }
                else
                {
                    result["message"] =
                        "Failed to add review";

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(
                            result);

                    response->setStatusCode(
                        drogon::k500InternalServerError);

                    callback(response);
                }
            }
        );
    }

    // Get reviews for a product
    void getProductReviews(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        auto productIdString =
            req->getParameter("productId");

        if (productIdString.empty())
        {
            Json::Value error;
            error["message"] =
                "productId is required";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

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
            Json::Value error;
            error["message"] =
                "Invalid productId";

            auto response =
                drogon::HttpResponse::newHttpJsonResponse(error);

            response->setStatusCode(
                drogon::k400BadRequest);

            callback(response);
            return;
        }

        service.getProductReviews(
            productId,

            [callback](const drogon::orm::Result& result)
            {
                Json::Value reviews(Json::arrayValue);

                for (const auto& row : result)
                {
                    Json::Value review;

                    review["id"] =
                        row["id"].as<int>();

                    review["userId"] =
                        row["user_id"].as<int>();

                    review["userName"] =
                        row["user_name"].as<std::string>();

                    review["productId"] =
                        row["product_id"].as<int>();

                    review["rating"] =
                        row["rating"].as<int>();

                    review["comment"] =
                        row["comment"].as<std::string>();

                    review["createdAt"] =
                        row["created_at"].as<std::string>();

                    reviews.append(review);
                }

                auto response =
                    drogon::HttpResponse::newHttpJsonResponse(
                        reviews);

                callback(response);
            }
        );
    }
};