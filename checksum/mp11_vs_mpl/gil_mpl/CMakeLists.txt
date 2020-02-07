cmake_minimum_required(VERSION 3.10)
project(gil_mpl
  LANGUAGES CXX
  DESCRIPTION "Test Boost.GIL version based on Boost.MPL")

set(_gil_include "D:\\boost.win.original")

foreach(_cpp
    mpl_bgr121_checksum.cpp
    mpl_bgr121_pixel.cpp)

    get_filename_component(_target ${_cpp} NAME_WE)
    add_executable(${_target}
        ${_target}.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../sample_image.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../utility.hpp)
    target_compile_definitions(${_target} PRIVATE BOOST_GIL_USE_CONCEPT_CHECK=1)
    target_include_directories(${_target} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../)
    target_include_directories(${_target} PRIVATE ${_gil_include})
endforeach()
