cmake_minimum_required(VERSION 3.28)

project(SnakeECS VERSION 0.0.1 DESCRIPTION "SnakeECS | An Entity-Component System." HOMEPAGE_URL "https://github.com/PraisePancakes/SnakeECS" LANGUAGES CXX)
add_executable(${PROJECT_NAME} main.cpp) 
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable)
endif()


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

