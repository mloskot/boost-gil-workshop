#
# Copyright (c) 2017 Mateusz Loskot <mateusz at loskot dot net>
# All rights reserved.
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 3.5.1)
project(boost-workshop CXX)

option(USE_BOOST_STAGE "Use Boost from current source tree and libraries from stage, unless BOOST_ROOT specified." ON)

#-----------------------------------------------------------------------------
# Dependency: Boost
# - look for stage Build
# - look for default installation location
# - look for location specified with BOOST_ROOT
#-----------------------------------------------------------------------------
if(USE_BOOST_STAGE AND NOT DEFINED BOOST_ROOT AND NOT DEFINED ENV{BOOST_ROOT})
  get_filename_component(_boost_root ../../ ABSOLUTE)
  if(EXISTS ${_boost_root}/boost-build.jam)
    set(BOOST_ROOT ${_boost_root})
    message(STATUS "Using Boost libraries from stage directory in BOOST_ROOT=${BOOST_ROOT}")
  endif()
endif()

set(BOOST_ROOT "C:\\local\\boost_1_66_0")
set(Boost_DETAILED_FAILURE_MSG ON)
if(MSVC)
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()
find_package(Boost 1.70 REQUIRED)

#-----------------------------------------------------------------------------
# Programs
#-----------------------------------------------------------------------------

add_executable(bit_aligned_pixel bit_aligned_pixel.cpp)
target_link_libraries(bit_aligned_pixel PRIVATE Boost::boost)

add_executable(image_crc image_crc.cpp sample_image_data.cpp)
target_link_libraries(image_crc PRIVATE Boost::boost)

add_executable(view_pixel_type view_pixel_type.cpp)
target_link_libraries(view_pixel_type PRIVATE Boost::boost)
