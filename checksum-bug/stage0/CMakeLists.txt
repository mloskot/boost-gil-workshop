foreach(_cpp
    stage0.cpp)
    get_filename_component(_target ${_cpp} NAME_WE)
    add_executable(${_target}
        ${_target}.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../utility.hpp)
    target_compile_definitions(${_target} PRIVATE BOOST_GIL_USE_CONCEPT_CHECK=1)
    target_include_directories(${_target} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../)
    if(_gil_include)
      target_include_directories(${_target} PRIVATE ${_gil_include})
    endif()
  unset(_cpp)
endforeach()
