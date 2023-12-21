cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)

project(dtest)

add_executable(dtest)

target_sources(dtest PRIVATE
    dtest/dtest.cpp
)