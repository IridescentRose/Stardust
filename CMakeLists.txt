cmake_minimum_required(VERSION 3.15)
project(Stardust)

set(CMAKE_CXX_STANDARD 17)

add_executable(StardustC src/main.cpp src/Tokenizer.hpp src/Tokenizer.cpp src/parse/Parser.hpp src/parse/Parser.cpp)
target_link_libraries(StardustC stdc++)