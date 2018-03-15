cmake_minimum_required(VERSION 3.5)
project(boost-gil-workshop CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.66 REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(TIFF REQUIRED)

set(gil_io_libraries
  Boost::boost
  PNG::PNG
  TIFF::TIFF
  ${JPEG_LIBRARIES})

set(gil_samples
  read_image_pnm)

foreach(sample IN ITEMS ${gil_samples})
  message(STATUS "Building sample: ${sample}")
  add_executable(${sample} ${sample}.cpp)
  target_include_directories(${sample} PRIVATE ${JPEG_INCLUDE_DIR})
  target_link_libraries(${sample} PRIVATE ${gil_io_libraries})
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
endforeach()
