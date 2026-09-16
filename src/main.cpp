#include <drogon/drogon.h>

#include "controller/UserController.h"
#include "controller/ProductController.h"

int main()
{
    try
    {
        // PostgreSQL connection
        auto dbClient = drogon::orm::DbClient::newPgClient(
            "host=127.0.0.1 port=5432 "
            "dbname=malinimart "
            "user=postgres "
            "password=Malini@2026",
            1
        );

        // Test database connection
        dbClient->execSqlSync("SELECT 1");

        std::cout << "Connected to PostgreSQL successfully!"
                  << std::endl;

        // Create controllers
        UserController userController(dbClient);
        ProductController productController(dbClient);

        // =================================
        // REGISTER USER
        // =================================
        drogon::app().registerHandler(
            "/users/register",
            [&userController](
                const drogon::HttpRequestPtr& req,
                std::function<void(
                    const drogon::HttpResponsePtr&)>&& callback)
            {
                userController.registerUser(
                    req,
                    std::move(callback));
            },
            {drogon::Post}
        );

        // =================================
        // LOGIN USER
        // =================================
        drogon::app().registerHandler(
            "/users/login",
            [&userController](
                const drogon::HttpRequestPtr& req,
                std::function<void(
                    const drogon::HttpResponsePtr&)>&& callback)
            {
                userController.loginUser(
                    req,
                    std::move(callback));
            },
            {drogon::Post}
        );

        // =================================
        // ADD PRODUCT
        // =================================
        drogon::app().registerHandler(
            "/products/add",
            [&productController](
                const drogon::HttpRequestPtr& req,
                std::function<void(
                    const drogon::HttpResponsePtr&)>&& callback)
            {
                productController.addProduct(
                    req,
                    std::move(callback));
            },
            {drogon::Post}
        );

        // =================================
        // GET ALL PRODUCTS
        // =================================
        drogon::app().registerHandler(
            "/products",
            [&productController](
                const drogon::HttpRequestPtr& req,
                std::function<void(
                    const drogon::HttpResponsePtr&)>&& callback)
            {
                productController.getAllProducts(
                    req,
                    std::move(callback));
            },
            {drogon::Get}
        );

        std::cout << "Server running on port 8080..."
                  << std::endl;

        // Start server
        drogon::app()
            .addListener("0.0.0.0", 8080)
            .run();
    }
    catch (const drogon::orm::DrogonDbException& e)
    {
        std::cerr << "Database connection failed: "
                  << e.base().what()
                  << std::endl;

        return 1;
    }

    return 0;
}