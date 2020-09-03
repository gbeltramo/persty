cmake_minimum_required(VERSION 3.8)
project(module_cpp_binding VERSION 0.1
                           LANGUAGES CXX)

find_package(pybind11)

pybind11_add_module(minibox MODULE minibox.cpp)
target_compile_features(minibox PUBLIC cxx_std_17)


if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    set(CMAKE_CXX_FLAGS "-O3")
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    set(CMAKE_CXX_FLAGS "/O2")
endif()
