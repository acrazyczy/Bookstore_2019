cmake_minimum_required(VERSION 3.10.2)
project(code)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Ofast")
include_directories(
        source
)
set(src_dir
        source/main.cpp
        source/bookstore.cpp source/bookstore.h source/hash_table.cpp source/hash_table.h source/constraint.h source/book_list.cpp source/book_list.h source/constraint.cpp)
add_executable(code ${src_dir})