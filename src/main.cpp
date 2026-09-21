#include <drogon/drogon.h>
#include <iostream>
#include <memory>

#include "controller/UserController.h"
#include "controller/ProductController.h"
#include "controller/CartController.h"
#include "controller/OrderController.h"
#include "controller/AdminController.h"
#include "controller/ReviewController.h"

int main()
{
    // Serve frontend files from the public folder
    drogon::app().setDocumentRoot("./public");

    // Load PostgreSQL configuration
    drogon::app().loadConfigFile("config.json");

    std::cout << "Config loaded" << std::endl;


    // =========================================================
    // FRONTEND - LOGIN PAGE
    // =========================================================

    drogon::app().registerHandler(
        "/",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            auto response =
                drogon::HttpResponse::newFileResponse(
                    "./public/index.html");

            callback(response);
        },
        {drogon::Get}
    );


    // =========================================================
    // USER REGISTER
    // =========================================================

    drogon::app().registerHandler(
        "/users/register",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            UserController controller(
                drogon::app().getDbClient("default"));

            controller.registerUser(
                req,
                std::move(callback));
        },
        {drogon::Post}
    );


    // =========================================================
    // USER LOGIN
    // =========================================================

    drogon::app().registerHandler(
        "/users/login",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            UserController controller(
                drogon::app().getDbClient("default"));

            controller.loginUser(
                req,
                std::move(callback));
        },
        {drogon::Post}
    );


    // =========================================================
    // ADD PRODUCT
    // =========================================================

    drogon::app().registerHandler(
        "/products/add",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            ProductController controller(
                drogon::app().getDbClient("default"));

            controller.addProduct(
                req,
                std::move(callback));
        },
        {drogon::Post}
    );


    // =========================================================
    // GET ALL PRODUCTS
    // =========================================================

    drogon::app().registerHandler(
        "/products",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            ProductController controller(
                drogon::app().getDbClient("default"));

            controller.getAllProducts(
                req,
                std::move(callback));
        },
        {drogon::Get}
    );


    // =========================================================
    // SEARCH PRODUCTS
    // =========================================================

    drogon::app().registerHandler(
        "/products/search",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            ProductController controller(
                drogon::app().getDbClient("default"));

            controller.searchProducts(
                req,
                std::move(callback));
        },
        {drogon::Get}
    );


    // =========================================================
    // UPDATE PRODUCT
    // =========================================================

    drogon::app().registerHandler(
        "/products/update",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            ProductController controller(
                drogon::app().getDbClient("default"));

            controller.updateProduct(
                req,
                std::move(callback));
        },
        {drogon::Put}
    );


    // =========================================================
    // DELETE PRODUCT
    // =========================================================

    drogon::app().registerHandler(
        "/products/delete",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            ProductController controller(
                drogon::app().getDbClient("default"));

            controller.deleteProduct(
                req,
                std::move(callback));
        },
        {drogon::Delete}
    );


    // =========================================================
    // ADD TO CART
    // =========================================================

    drogon::app().registerHandler(
        "/cart/add",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            CartController controller(
                drogon::app().getDbClient("default"));

            controller.addToCart(
                req,
                std::move(callback));
        },
        {drogon::Post}
    );


    // =========================================================
    // GET CART
    // =========================================================

    drogon::app().registerHandler(
        "/cart",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            CartController controller(
                drogon::app().getDbClient("default"));

            controller.getCart(
                req,
                std::move(callback));
        },
        {drogon::Get}
    );


    // =========================================================
    // UPDATE CART
    // =========================================================

    drogon::app().registerHandler(
        "/cart/update",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            CartController controller(
                drogon::app().getDbClient("default"));

            controller.updateCart(
                req,
                std::move(callback));
        },
        {drogon::Put}
    );


    // =========================================================
    // REMOVE FROM CART
    // =========================================================

    drogon::app().registerHandler(
        "/cart/remove",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            CartController controller(
                drogon::app().getDbClient("default"));

            controller.removeFromCart(
                req,
                std::move(callback));
        },
        {drogon::Delete}
    );


    // =========================================================
    // CREATE ORDER
    // =========================================================

    drogon::app().registerHandler(
        "/orders",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            OrderController controller(
                drogon::app().getDbClient("default"));

            controller.createOrder(
                req,
                std::move(callback));
        },
        {drogon::Post}
    );


    // =========================================================
    // CHECKOUT
    // =========================================================

    drogon::app().registerHandler(
        "/checkout",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            auto controller =
                std::make_shared<OrderController>(
                    drogon::app().getDbClient("default"));

            controller->checkout(
                req,
                [controller,
                 callback = std::move(callback)]
                (const drogon::HttpResponsePtr& response)
                {
                    callback(response);
                }
            );
        },
        {drogon::Post}
    );


    // =========================================================
    // BUYER ORDER HISTORY
    // =========================================================

    drogon::app().registerHandler(
        "/orders/history",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            OrderController controller(
                drogon::app().getDbClient("default"));

            controller.getOrders(
                req,
                std::move(callback));
        },
        {drogon::Get}
    );


    // =========================================================
    // SELLER ORDERS
    // =========================================================

    drogon::app().registerHandler(
        "/seller/orders",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            OrderController controller(
                drogon::app().getDbClient("default"));

            controller.getSellerOrders(
                req,
                std::move(callback));
        },
        {drogon::Get}
    );


    // =========================================================
    // ADMIN - ALL USERS
    // =========================================================

    drogon::app().registerHandler(
        "/admin/users",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            AdminController controller(
                drogon::app().getDbClient("default"));

            controller.getAllUsers(
                req,
                std::move(callback));
        },
        {drogon::Get}
    );


    // =========================================================
    // ADMIN - ALL ORDERS
    // =========================================================

    drogon::app().registerHandler(
        "/admin/orders",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            AdminController controller(
                drogon::app().getDbClient("default"));

            controller.getAllOrders(
                req,
                std::move(callback));
        },
        {drogon::Get}
    );


    // =========================================================
    // ADMIN - DELETE PRODUCT
    // =========================================================

    drogon::app().registerHandler(
        "/admin/products",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            AdminController controller(
                drogon::app().getDbClient("default"));

            controller.deleteProduct(
                req,
                std::move(callback));
        },
        {drogon::Delete}
    );


    // =========================================================
    // ADD REVIEW
    // =========================================================

    drogon::app().registerHandler(
        "/reviews/add",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            ReviewController controller(
                drogon::app().getDbClient("default"));

            controller.addReview(
                req,
                std::move(callback));
        },
        {drogon::Post}
    );


    // =========================================================
    // GET REVIEWS
    // =========================================================

    drogon::app().registerHandler(
        "/reviews",
        [](const drogon::HttpRequestPtr& req,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            ReviewController controller(
                drogon::app().getDbClient("default"));

            controller.getProductReviews(
                req,
                std::move(callback));
        },
        {drogon::Get}
    );


    // =========================================================
    // START SERVER
    // =========================================================

    drogon::app().addListener(
        "0.0.0.0",
        8080
    );

    std::cout << "Listener added" << std::endl;
    std::cout << "Starting MaliniMart..." << std::endl;

    drogon::app().run();

    std::cout << "Server stopped" << std::endl;

    return 0;
}