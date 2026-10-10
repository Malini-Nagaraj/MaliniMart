#include <drogon/drogon.h>

#include "controller/UserController.h"
#include "controller/ProductController.h"
#include "controller/CartController.h"
#include "controller/OrderController.h"
#include "controller/AdminController.h"
#include "controller/ReviewController.h"

int main()
{
    // ==============================
    // 1. Frontend folder
    // ==============================
    drogon::app().setDocumentRoot("./public");
    drogon::app().enableSession(3600);
    

    // ==============================
    // 2. PostgreSQL connection
    // ==============================
    auto dbClient = drogon::orm::DbClient::newPgClient(
        "host=127.0.0.1 "
        "port=5432 "
        "dbname=malinimart "
        "user=postgres "
        "password=Malini@2026",
                1
    );

    if (!dbClient)
    {
        std::cout << "Database client creation failed!" << std::endl;
        return 1;
    }

    std::cout << "Database client created successfully!" << std::endl;

    // ==============================
    // 3. Controllers
    // ==============================
    UserController userController(dbClient);
    ProductController productController(dbClient);
    CartController cartController(dbClient);
    OrderController orderController(dbClient);
    AdminController adminController(dbClient);
    ReviewController reviewController(dbClient);

    // ==============================
    // 4. USER REGISTER
    // ==============================
    drogon::app().registerHandler(
        "/users/register",
        [&userController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            userController.registerUser(req, std::move(callback));
        },
        {drogon::Post}
    );

    // ==============================
    // 5. USER LOGIN
    // ==============================
    drogon::app().registerHandler(
        "/users/login",
        [&userController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            userController.loginUser(req, std::move(callback));
        },
        {drogon::Post}
    );

    // ==============================
    // 6. GET PRODUCTS
    // ==============================
    drogon::app().registerHandler(
        "/products",
        [&productController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            productController.getAllProducts(req, std::move(callback));
        },
        {drogon::Get}
    );

    // ==============================
    // 7. SEARCH PRODUCTS
    // ==============================
    drogon::app().registerHandler(
        "/products/search",
        [&productController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            productController.searchProducts(req, std::move(callback));
        },
        {drogon::Get}
    );

    // ==============================
    // 8. ADD PRODUCT
    // ==============================
    drogon::app().registerHandler(
        "/products/add",
        [&productController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            productController.addProduct(req, std::move(callback));
        },
        {drogon::Post}
    );

    // ==============================
    // 9. UPDATE PRODUCT
    // ==============================
    drogon::app().registerHandler(
        "/products/update",
        [&productController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            productController.updateProduct(req, std::move(callback));
        },
        {drogon::Put}
    );

    // ==============================
    // 10. DELETE PRODUCT
    // ==============================
    drogon::app().registerHandler(
        "/products/delete",
        [&productController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            productController.deleteProduct(req, std::move(callback));
        },
        {drogon::Delete}
    );

    // ==============================
    // 11. ADD TO CART
    // ==============================
    drogon::app().registerHandler(
        "/cart/add",
        [&cartController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            cartController.addToCart(req, std::move(callback));
        },
        {drogon::Post}
    );

    // ==============================
    // 12. GET CART
    // ==============================
    drogon::app().registerHandler(
        "/cart",
        [&cartController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            cartController.getCart(req, std::move(callback));
        },
        {drogon::Get}
    );

    // ==============================
    // 13. UPDATE CART
    // ==============================
    drogon::app().registerHandler(
        "/cart/update",
        [&cartController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            cartController.updateCart(req, std::move(callback));
        },
        {drogon::Put}
    );

    // ==============================
    // 14. REMOVE FROM CART
    // ==============================
    drogon::app().registerHandler(
        "/cart/remove",
        [&cartController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            cartController.removeFromCart(req, std::move(callback));
        },
        {drogon::Delete}
    );

    // ==============================
    // 15. CREATE ORDER
    // ==============================
    drogon::app().registerHandler(
        "/orders/create",
        [&orderController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            orderController.createOrder(req, std::move(callback));
        },
        {drogon::Post}
    );

    // ==============================
    // 16. GET ORDERS
    // ==============================
    drogon::app().registerHandler(
        "/orders",
        [&orderController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            orderController.getOrders(req, std::move(callback));
        },
        {drogon::Get}
    );
    
    // ==============================
    // ADMIN: GET USERS
    // ==============================
    drogon::app().registerHandler(
        "/admin/users",
        [&adminController](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            adminController.getUsers(req, std::move(callback));
        },
        {drogon::Get}
    );

    // ==============================
    // 17. HOME PAGE
    // ==============================
    drogon::app().registerHandler(
        "/",
        [](const drogon::HttpRequestPtr&,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            auto response =
                drogon::HttpResponse::newFileResponse(
                    "./public/index.html"
                );

            callback(response);
        },
        {drogon::Get}
    );

    // ==============================
    // 18. IMPORTANT: PORT 8080
    // ==============================
    drogon::app().addListener("0.0.0.0", 8080);

    std::cout << "Starting MaliniMart..." << std::endl;
    std::cout << "Open: http://localhost:8080" << std::endl;

    // ==============================
    // 19. START SERVER
    // ==============================
    std::cout << "ADMIN ROUTE REGISTERED" << std::endl;

    drogon::app().run();

    return 0;
}