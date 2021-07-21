cmake_minimum_required(VERSION 3.11)
project(persty_cpp_binding VERSION 2.0.0
                           LANGUAGES CXX)

#============================
if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    set(PYBIND11_CPP_STANDARD -std=c++14)
    set(CMAKE_CXX_FLAGS "-O3")
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    set(PYBIND11_CPP_STANDARD /std:c++14)
    set(CMAKE_CXX_FLAGS "/O2")
endif()

#============================
find_package(pybind11)

#============================
# UTIL LIBRARY
add_library(util STATIC
                 inc/util.hpp
                 src/util.cpp)
target_include_directories(util PUBLIC inc)
target_compile_features(util PUBLIC cxx_std_14)

#============================
# MINIBOX LIBRARY
add_library(minibox STATIC
                    inc/minibox.hpp
                    src/minibox.cpp)
target_include_directories(minibox PUBLIC inc)
target_link_libraries(minibox PRIVATE util)
target_compile_features(minibox PUBLIC cxx_std_14)

#============================
# DELAUNAY LIBRARY
add_library(delaunay STATIC
                     inc/delaunay.hpp
                     src/delaunay.cpp)
target_include_directories(delaunay PUBLIC inc)
#target_link_libraries(delaunay PRIVATE util)
target_compile_features(delaunay PUBLIC cxx_std_14)

#============================
# FINAL PYTHON MODULE USING PYBIND11
pybind11_add_module(binding MODULE binding.cpp)
target_link_libraries(binding PRIVATE util)
target_link_libraries(binding PRIVATE minibox)
target_link_libraries(binding PRIVATE delaunay)
target_compile_features(binding PUBLIC cxx_std_14)
