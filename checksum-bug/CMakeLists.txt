cmake_minimum_required(VERSION 3.10)
project(gil_checksum_bug LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  set(_gil_include "D:\\boost.win")
endif()

add_subdirectory(stage0)
add_subdirectory(stage1)
