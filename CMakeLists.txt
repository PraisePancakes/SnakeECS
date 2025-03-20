cmake_minimum_required(VERSION 3.28)

project(SnakeECS VERSION 0.0.1 DESCRIPTION "SnakeECS | An Entity-Component System." HOMEPAGE_URL "https://github.com/PraisePancakes/SnakeECS" LANGUAGES CXX)
add_executable(${PROJECT_NAME} main.cpp) 

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
