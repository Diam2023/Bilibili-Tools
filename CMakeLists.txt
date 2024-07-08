cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(BiliBiliTools VERSION 10 LANGUAGES CXX C)

add_subdirectory(BiliBiliTools)
add_subdirectory(third_party/drogon)
add_subdirectory(third_party/priority-timer-queue)

target_compile_definitions(libmono INTERFACE
    # MONO_PTQ_DEBUG
)

add_subdirectory(test)
