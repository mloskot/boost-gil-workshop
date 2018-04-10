cmake_minimum_required(VERSION 3.10)
project(boost-gil-workshop CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_CONAN "Use Conan to install dependencies" OFF)

#-----------------------------------------------------------------------------
# Dependency: Boost
#-----------------------------------------------------------------------------
if(NOT DEFINED BOOST_ROOT AND NOT DEFINED ENV{BOOST_ROOT})
  message(STATUS "Using Boost libraries from stage directory in BOOST_ROOT=${BOOST_ROOT}")
endif()

set(Boost_DETAILED_FAILURE_MSG ON)
if(MSVC)
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()
find_package(Boost 1.66 REQUIRED)
include_directories(${BOOST_ROOT}/libs/gil/include) # Help CMake-aware IDEs

#-----------------------------------------------------------------------------
# Dependency: libpng, libjpeg, libtiff via Vcpkg or Conan
#-----------------------------------------------------------------------------
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

#-----------------------------------------------------------------------------
# Compiler
#
# Follows https://svn.boost.org/trac10/wiki/Guidelines/WarningsGuidelines
#-----------------------------------------------------------------------------
if(MSVC)
  string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  add_compile_options(-W4)
  add_compile_options(-we4288) # For-loop scoping (this is the default)
  add_compile_options(-we4238) # don't take address of temporaries
  add_compile_options(-we4239) # don't bind temporaries to non-const references (Stephan's "Evil Extension")
  add_compile_options(-we4346) # require "typename" where the standard requires it
  add_compile_options(-FC) # We need absolute path in __FILE__
  add_compile_options(-Zc:forScope) # For loop scoping again (this is the default)
  add_definitions(-DVC_EXTRALEAN=1)
  add_definitions(-DWIN32_LEAN_AND_MEAN=1)
  add_definitions(-DNOMINMAX=1)
  add_definitions(-DNOGDI=1)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE=1)
  add_definitions(-D_SCL_SECURE_NO_DEPRECATE=1)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS=1)
  add_definitions(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1)
else()
  add_compile_options(-pedantic)
  add_compile_options(-Wall)
  add_compile_options(-Wextra)
  add_compile_options(-Wstrict-aliasing)
  add_compile_options(-fstrict-aliasing)
  add_compile_options(-Wconversion )
  add_compile_options(-Wsign-promo)
  add_compile_options(-Wfloat-equal)
  add_compile_options(-Wunused-parameter)
  add_compile_options(-Wshadow)
endif()

#-----------------------------------------------------------------------------
# Targets
#-----------------------------------------------------------------------------
set(gil_samples
  indexed_transparency
  indexed_image_raw
  read_image_pnm
  stitch_image_tiles)

file(GLOB _gil_headers ${BOOST_ROOT}/libs/gil/include/boost/gil/*.hpp)

foreach(sample IN ITEMS ${gil_samples})
  message(STATUS "Building sample: ${sample}")
  add_executable(${sample} ${_gil_headers} ${sample}.cpp)

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

add_subdirectory(crc)
