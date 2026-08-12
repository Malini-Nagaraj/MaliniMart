cmake_minimum_required(VERSION 3.25)

project(MaliniMart VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Drogon CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

add_executable(MaliniMart
    src/main.cc
    src/controller/UserController.cc
    src/service/UserService.cc
    src/database/Database.cc
    src/repository/UserRepository.cc
)


target_link_libraries(MaliniMart PRIVATE
    Drogon::Drogon
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    unofficial-sodium::sodium
    libpqxx::pqxx
)