#include <drogon/drogon.h>

#include "controller/UserController.h"
#include "controller/ProductController.h"
#include "controller/CartController.h"
#include "controller/OrderController.h"
#include "controller/AdminController.h"
#include "controller/ReviewController.h"

int main()
{
    drogon::app().createDbClient(
        "postgresql",
        "127.0.0.1",
        5432,
        "malinimart",
        "postgres",
        "Malini@2026",
        1,
        "",
        "default",
        false
    );

    auto dbClient = drogon::app().getDbClient("default");


    // =========================
    // CONTROLLERS
    // =========================

    UserController userController(dbClient);

    ProductController productController(dbClient);

    CartController cartController(dbClient);

    OrderController orderController(dbClient);

    AdminController adminController(dbClient);

    ReviewController reviewController(dbClient);


    // =========================
    // USER ROUTES
    // =========================

    drogon::app().registerHandler(
        "/users/register",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            userController.registerUser(
                req,
                std::move(callback));
        },

        {drogon::Post}
    );


    drogon::app().registerHandler(
        "/users/login",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            userController.loginUser(
                req,
                std::move(callback));
        },

        {drogon::Post}
    );


    // =========================
    // PRODUCT ROUTES
    // =========================

    drogon::app().registerHandler(
        "/products/add",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            productController.addProduct(
                req,
                std::move(callback));
        },

        {drogon::Post}
    );


    drogon::app().registerHandler(
        "/products",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            productController.getAllProducts(
                req,
                std::move(callback));
        },

        {drogon::Get}
    );


    drogon::app().registerHandler(
        "/products/search",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            productController.searchProducts(
                req,
                std::move(callback));
        },

        {drogon::Get}
    );


    // Update:
    // PUT /products/update?productId=1

    drogon::app().registerHandler(
        "/products/update",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            productController.updateProduct(
                req,
                std::move(callback));
        },

        {drogon::Put}
    );


    // Delete:
    // DELETE /products/delete?productId=1

    drogon::app().registerHandler(
        "/products/delete",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            productController.deleteProduct(
                req,
                std::move(callback));
        },

        {drogon::Delete}
    );


    // =========================
    // CART ROUTES
    // =========================

    drogon::app().registerHandler(
        "/cart/add",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            cartController.addToCart(
                req,
                std::move(callback));
        },

        {drogon::Post}
    );


    // GET /cart?userId=1

    drogon::app().registerHandler(
        "/cart",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            cartController.getCart(
                req,
                std::move(callback));
        },

        {drogon::Get}
    );


    // PUT /cart/update?cartId=1

    drogon::app().registerHandler(
        "/cart/update",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            cartController.updateCart(
                req,
                std::move(callback));
        },

        {drogon::Put}
    );


    // DELETE /cart/remove?cartId=1

    drogon::app().registerHandler(
        "/cart/remove",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            cartController.removeFromCart(
                req,
                std::move(callback));
        },

        {drogon::Delete}
    );


    // =========================
    // ORDER ROUTES
    // =========================

    drogon::app().registerHandler(
        "/orders",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            orderController.createOrder(
                req,
                std::move(callback));
        },

        {drogon::Post}
    );


    drogon::app().registerHandler(
        "/checkout",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            orderController.checkout(
                req,
                std::move(callback));
        },

        {drogon::Post}
    );


    drogon::app().registerHandler(
        "/orders/history",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            orderController.getOrders(
                req,
                std::move(callback));
        },

        {drogon::Get}
    );


    drogon::app().registerHandler(
        "/seller/orders",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            orderController.getSellerOrders(
                req,
                std::move(callback));
        },

        {drogon::Get}
    );


    // =========================
    // ADMIN ROUTES
    // =========================

    drogon::app().registerHandler(
        "/admin/users",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            adminController.getAllUsers(
                req,
                std::move(callback));
        },

        {drogon::Get}
    );


    drogon::app().registerHandler(
        "/admin/orders",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            adminController.getAllOrders(
                req,
                std::move(callback));
        },

        {drogon::Get}
    );


    // DELETE /admin/products?productId=1

    drogon::app().registerHandler(
        "/admin/products",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            adminController.deleteProduct(
                req,
                std::move(callback));
        },

        {drogon::Delete}
    );


    // =========================
    // REVIEW ROUTES
    // =========================

    drogon::app().registerHandler(
        "/reviews/add",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            reviewController.addReview(
                req,
                std::move(callback));
        },

        {drogon::Post}
    );


    drogon::app().registerHandler(
        "/reviews",

        [&](const drogon::HttpRequestPtr& req,
            std::function<void(
                const drogon::HttpResponsePtr&)>&& callback)
        {
            reviewController.getProductReviews(
                req,
                std::move(callback));
        },

        {drogon::Get}
    );


    // =========================
    // START SERVER
    // =========================

    drogon::app().addListener(
        "0.0.0.0",
        8080
    );

    std::cout
        << "MaliniMart server running on port 8080..."
        << std::endl;

    drogon::app().run();

    return 0;
}