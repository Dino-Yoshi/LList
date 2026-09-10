cmake_minimum_required(VERSION 3.20)
project(LList LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
include(FetchContent)

add_library(LList STATIC LList.cpp LList.h)
target_include_directories(LList PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

FetchContent_Declare(
        Catch2
        URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.8.1.tar.gz
)
FetchContent_MakeAvailable(Catch2)

add_executable(LListTests tests/LListTests.cpp)
target_link_libraries(LListTests PRIVATE Catch2::Catch2WithMain)
target_include_directories(LListTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(Catch)
catch_discover_tests(LListTests)
