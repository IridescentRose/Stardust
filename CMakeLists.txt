cmake_minimum_required(VERSION 3.15)
project(Stardust)

set(CMAKE_CXX_STANDARD 17)

add_executable(StardustI src/main.cpp)
target_link_libraries(StardustI stdc++)