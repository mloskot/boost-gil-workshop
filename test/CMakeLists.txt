add_executable(gil_gdal_test
  gil_gdal_test.cpp
  ${GIL_HPP}
  ${GIL_EXT_IO_HPP}
  ${GIL_EXT_IO_DETAIL_HPP}
  ${GIL_EXT_IO_FORMATS_GDAL_HPP})

# Help IDE users to find Boost.GIL headers
source_group("Header Files\\gil" FILES ${GIL_HPP})
source_group("Header Files\\gil\\extension\\io" FILES ${GIL_EXT_IO_HPP})
source_group("Header Files\\gil\\extension\\io\\detail" FILES ${GIL_EXT_IO_DETAIL_HPP})
source_group("Header Files\\gil\\extension\\io\\formats\\gdal" FILES ${GIL_EXT_IO_FORMATS_GDAL_HPP})
