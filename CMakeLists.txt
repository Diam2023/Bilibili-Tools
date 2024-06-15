cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

project(BiliBiliTools)

add_subdirectory(BiliBiliTools)

find_package(Drogon CONFIG REQUIRED)

add_subdirectory(test)
