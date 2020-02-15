cmake_minimum_required(VERSION 3.10)
project(gil_checksum_bug_mp11_vs_mpl LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(_gil_include "D:\\boost.win")

add_subdirectory(gil_mp11)
add_subdirectory(gil_mpl)
