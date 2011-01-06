add_executable(gil_gdal_test
  gil_gdal_test.cpp
  ${GIL_HPP}
  ${GIL_EXT_IO_HPP}
  ${GIL_EXT_IO_DETAIL_HPP}
  ${GIL_EXT_IO_FORMATS_BMP_HPP}
  ${GIL_EXT_IO_FORMATS_JPEG_HPP}
  ${GIL_EXT_IO_FORMATS_GDAL_HPP}
  ${GIL_EXT_IO_FORMATS_PNG_HPP}
  ${GIL_EXT_IO_FORMATS_PNM_HPP}
  ${GIL_EXT_IO_FORMATS_TARGA_HPP}
  ${GIL_EXT_IO_FORMATS_TIFF_HPP})

target_link_libraries(gil_gdal_test ${GDAL_LIBRARY})

# Help IDE users to find Boost.GIL headers
source_group("Header Files\\gil" FILES ${GIL_HPP})
source_group("Header Files\\gil\\extension\\io" FILES ${GIL_EXT_IO_HPP})
source_group("Header Files\\gil\\extension\\io\\detail" FILES ${GIL_EXT_IO_DETAIL_HPP})
source_group("Header Files\\gil\\extension\\io\\formats\\bmp" FILES ${GIL_EXT_IO_FORMATS_BMP_HPP})
source_group("Header Files\\gil\\extension\\io\\formats\\jpeg" FILES ${GIL_EXT_IO_FORMATS_JPEG_HPP})
source_group("Header Files\\gil\\extension\\io\\formats\\gdal" FILES ${GIL_EXT_IO_FORMATS_GDAL_HPP})
source_group("Header Files\\gil\\extension\\io\\formats\\png" FILES ${GIL_EXT_IO_FORMATS_PNG_HPP})
source_group("Header Files\\gil\\extension\\io\\formats\\pnm" FILES ${GIL_EXT_IO_FORMATS_PNM_HPP})
source_group("Header Files\\gil\\extension\\io\\formats\\targa" FILES ${GIL_EXT_IO_FORMATS_TARGA_HPP})
source_group("Header Files\\gil\\extension\\io\\formats\\tiff" FILES ${GIL_EXT_IO_FORMATS_TIFF_HPP})

