cmake_minimum_required(VERSION 3.20)

project(MaliniMart)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon CONFIG REQUIRED)

add_executable(MaliniMart
    src/main.cpp
)

target_link_libraries(MaliniMart PRIVATE Drogon::Drogon)