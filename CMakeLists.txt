cmake_minimum_required(VERSION 3.5)
project(boost-gil-workshop CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_BOOST_STAGE "Use Boost from current source tree and libraries from stage, unless BOOST_ROOT specified." ON)
option(USE_CONAN "Use Conan to install dependencies" OFF)

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

set (BOOST_ROOT "D:/dev/boost/boost")
set(Boost_DETAILED_FAILURE_MSG ON)
if(MSVC)
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()
find_package(Boost 1.66 REQUIRED)

if(USE_CONAN)
  # Download automatically, you can also just copy the conan.cmake file
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.9/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake")
  endif()

  # NOTE: See RelWithDebInfo for Release builds, http://docs.conan.io/en/latest/howtos/vs2017_cmake.html
  set(_build_type_saved ${CMAKE_BUILD_TYPE})
  if(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CMAKE_BUILD_TYPE "Release")
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)
  conan_cmake_run(CONANFILE conanfile.txt BASIC_SETUP CMAKE_TARGETS)

  set(CMAKE_BUILD_TYPE ${_build_type_saved})
else()
  find_package(JPEG REQUIRED)
  find_package(PNG REQUIRED)
  find_package(TIFF REQUIRED)
endif()

set(gil_samples
  read_image_pnm
  tile_mosaic)

foreach(sample IN ITEMS ${gil_samples})
  message(STATUS "Building sample: ${sample}")
  add_executable(${sample} ${sample}.cpp)
  if(MSVC)
    string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    target_compile_options(${sample} PRIVATE -W4)
    target_compile_definitions(${sample} PRIVATE _CRT_NONSTDC_NO_DEPRECATE=1)
    target_compile_definitions(${sample} PRIVATE _SCL_SECURE_NO_DEPRECATE=1)
    target_compile_definitions(${sample} PRIVATE _CRT_SECURE_NO_WARNINGS=1)
    target_compile_definitions(${sample} PRIVATE _SCL_SECURE_NO_WARNINGS=1)
    target_compile_definitions(${sample} PRIVATE BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1)
  else()
    target_compile_options(${sample} PRIVATE -Wall)
    target_compile_options(${sample} PRIVATE -pedantic)
  endif()

  # Unfortunately, ALIAS of imported target is not supported
  # see https://github.com/conan-io/conan/issues/2125
  if (USE_CONAN)
    target_link_libraries(${sample}
      PRIVATE
      Boost::boost
      Boost::disable_autolinking
      CONAN_PKG::libjpeg
      CONAN_PKG::libpng
      CONAN_PKG::libtiff)
  else()
    target_link_libraries(${sample}
      PRIVATE
      Boost::boost
      Boost::disable_autolinking
      PNG::PNG
      TIFF::TIFF
      ${JPEG_LIBRARIES})
      target_include_directories(${sample} PRIVATE ${JPEG_INCLUDE_DIR})
  endif()
endforeach()
