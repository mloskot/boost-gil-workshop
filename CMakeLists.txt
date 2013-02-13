cmake_minimum_required(VERSION 2.8)
project(boost-gil-workshop)

# Additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Boost is required
find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif() 

find_package(TIFF)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_SCL_SECURE_NO_DEPRECATE)
endif()

message(STATUS "Using Boost headers from ${Boost_INCLUDE_DIR}")

set(GIL_DIR ${Boost_INCLUDE_DIR}/boost/gil)
message(STATUS "Using Boost.GIL headers from ${Boost_INCLUDE_DIR}/boost/gil")

set(GIL_EXT_DIR ${CMAKE_SOURCE_DIR}/boost/gil/extension)
message(STATUS "Using Boost.GIL extensions headers from ${GIL_EXT_DIR}")

set(GIL_EXT_IO_DIR ${GIL_EXT_DIR}/io_new)
set(GIL_EXT_TOOLBOX_DIR ${GIL_EXT_DIR}/toolbox)

# GIL headers
file(GLOB GIL_HPP "${GIL_DIR}/*.hpp")
file(GLOB GIL_EXT_TOOLBOX_HPP "${GIL_EXT_TOOLBOX_DIR}/*.hpp")
file(GLOB GIL_EXT_IO_HPP "${GIL_EXT_IO_DIR}/*.hpp")
file(GLOB GIL_EXT_IO_DETAIL_HPP "${GIL_EXT_IO_DIR}/detail/*.hpp")
file(GLOB GIL_EXT_IO_FORMATS_BMP_HPP "${GIL_EXT_IO_DIR}/formats/bmp/*.hpp")
file(GLOB GIL_EXT_IO_FORMATS_JPEG_HPP "${GIL_EXT_IO_DIR}/formats/jpeg/*.hpp")
file(GLOB GIL_EXT_IO_FORMATS_PNG_HPP "${GIL_EXT_IO_DIR}/formats/png/*.hpp")
file(GLOB GIL_EXT_IO_FORMATS_PNM_HPP "${GIL_EXT_IO_DIR}/formats/pnm/*.hpp")
file(GLOB GIL_EXT_IO_FORMATS_TARGA_HPP "${GIL_EXT_IO_DIR}/formats/targa/*.hpp")
file(GLOB GIL_EXT_IO_FORMATS_TIFF_HPP "${GIL_EXT_IO_DIR}/formats/tiff/*.hpp")

if(MSVC)
  list(APPEND GIL_ALL_HPP ${GIL_HPP})
  list(APPEND GIL_ALL_HPP ${GIL_EXT_TOOLBOX_HPP})
  list(APPEND GIL_ALL_HPP ${GIL_EXT_IO_HPP})
  list(APPEND GIL_ALL_HPP ${GIL_EXT_IO_DETAIL_HPP})
  list(APPEND GIL_ALL_HPP ${GIL_EXT_IO_FORMATS_BMP_HPP})
  list(APPEND GIL_ALL_HPP ${GIL_EXT_IO_FORMATS_JPEG_HPP})
  list(APPEND GIL_ALL_HPP ${GIL_EXT_IO_FORMATS_PNG_HPP})
  list(APPEND GIL_ALL_HPP ${GIL_EXT_IO_FORMATS_PNM_HPP})
  list(APPEND GIL_ALL_HPP ${GIL_EXT_IO_FORMATS_TARGA_HPP})
  list(APPEND GIL_ALL_HPP ${GIL_EXT_IO_FORMATS_TIFF_HPP})
endif()

# Configure test programs, also adds the collected headers
add_subdirectory(test)
