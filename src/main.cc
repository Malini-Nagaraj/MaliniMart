#include <drogon/drogon.h>
#include <iostream>

#include "controller/UserController.h"
#include "service/UserService.h"
#include "model/User.h"
#include "database/Database.h"

int main()
{
    UserController userController;
    userController.hello();

    User user;
    user.name = "Malini";
    user.email = "malini@gmail.com";
    user.password = "123456";

    UserService userService;
    userService.registerUser(user);

    try
    {
        auto client = Database::getClient();
        std::cout << "Connected to PostgreSQL successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    // Serve static files
    drogon::app().setDocumentRoot("./public");

    // Start server
    drogon::app().addListener("0.0.0.0", 8080);

    std::cout << "Server Running..." << std::endl;
    std::cout << "http://127.0.0.1:8080" << std::endl;

    drogon::app().run();

    return 0;
}