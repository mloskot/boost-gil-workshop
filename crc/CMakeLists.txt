set(gil_crc_samples
  image_crc)

add_executable(image_crc image_crc.cpp sample_image.cpp)

foreach(sample IN ITEMS ${gil_crc_samples})
  message(STATUS "Building sample: ${sample}")
  if(MSVC)
    string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    target_compile_options(${sample} PRIVATE -W4 -FC) # We need absolute path in __FILE__
    target_compile_definitions(${sample} PRIVATE _CRT_NONSTDC_NO_DEPRECATE=1)
    target_compile_definitions(${sample} PRIVATE _SCL_SECURE_NO_DEPRECATE=1)
    target_compile_definitions(${sample} PRIVATE _CRT_SECURE_NO_WARNINGS=1)
    target_compile_definitions(${sample} PRIVATE _SCL_SECURE_NO_WARNINGS=1)
    target_compile_definitions(${sample} PRIVATE BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1)
  else()
    target_compile_options(${sample} PRIVATE -Wall)
    target_compile_options(${sample} PRIVATE -pedantic)
  endif()
  target_link_libraries(${sample} PRIVATE Boost::boost)
endforeach()
