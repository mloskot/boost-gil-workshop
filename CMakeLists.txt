cmake_minimum_required(VERSION 2.8)
project(boost-gil-extension)

# Additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Boost is required
find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif() 

# GDAL is required
find_package(GDAL 1.6)
if(GDAL_FOUND)
  include_directories(${GDAL_INCLUDE_DIR})
endif() 

# Location of the Boost.GIL headers
set(GIL_DIR ${Boost_INCLUDE_DIR}/boost/gil)
# Location of the Boost.GIL IOv2 headers
set(GIL_EXT_IO_DIR ${GIL_DIR}/extension/io_new)
# Location of the boost-gil-extension headers
set(GDAL_GIL_EXT_IO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gil/extension/io)

# Find headers
file(GLOB GIL_HPP "${GIL_DIR}/*.hpp")
file(GLOB GIL_EXT_IO_HPP "${GIL_EXT_IO_DIR}/*.hpp")
file(GLOB GIL_EXT_IO_DETAIL_HPP "${GIL_EXT_IO_DIR}/detail/*.hpp")
file(GLOB GIL_EXT_IO_GDAL_HPP "${GDAL_GIL_EXT_IO_DIR}/*.hpp")
file(GLOB GIL_EXT_IO_FORMATS_GDAL_HPP "${GDAL_GIL_EXT_IO_DIR}/formats/gdal/*.hpp")

list(APPEND GIL_EXT_IO_HPP ${GIL_EXT_IO_GDAL_HPP})

# Configure test programs, also adds the collected headers
add_subdirectory(test)
