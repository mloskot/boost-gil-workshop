//
// Copyright 2010-2011 (c) Mateusz Loskot <mateusz@loskot.net>
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
//
#ifndef BOOST_GIL_EXTENSION_IO_GDAL_TAGS_HPP
#define BOOST_GIL_EXTENSION_IO_GDAL_TAGS_HPP

#include "detail/base.hpp"

namespace boost { namespace gil {

/// Defines tag for images accessed using GDAL.
struct gdal_tag : format_tag {};

/// For a description see: http://www.gdal.org

/// Defines type to store image (or block of image) dimension property.
/// Image dimension is width, height, bits depth, number of channels, etc.
struct gdal_dimension : property_base<int> {};

/// Read settings for images accessed through GDAL.
///
/// The structure can be used for all read_xxx functions, except read_image_info.
template<>
struct image_read_settings<gdal_tag> : public image_read_settings_base
{
    /// Default constructor
    image_read_settings()
        : image_read_settings_base()
    {}

    /// Constructor
    /// \param top_left Top left coordinate for reading partial image.
    /// \param dim Dimensions for reading partial image.
    image_read_settings(point_t const& top_left, point_t const& dim)
        : image_read_settings_base(top_left, dim)
    {}
};

/// Read information for images accessed using GDAL.
///
/// The structure is returned when using read_image_info.
template<>
struct image_read_info<gdal_tag>
{
    image_read_info()
        : width_(gdal_dimension::type())
        , height_(gdal_dimension::type())
        , bits_per_channel(gdal_dimension::type())
        , channels_per_pixel(gdal_dimension::type())
        , block_width(gdal_dimension::type())
        , block_height(gdal_dimension::type())
    {}

    /// Number of rows of pixels in the image.
    gdal_dimension::type width_;

    /// Number of columns in the image, i.e., the number of pixels per row.
    gdal_dimension::type height_;

    /// Number of bits per each color component (channel).
    /// @todo What is heterogeneous channels?
    gdal_dimension::type bits_per_channel;

    /// Number of separate channels (color planes) in the image.
    gdal_dimension::type channels_per_pixel;

    /// Number of rows of pixels in the natural block.
    /// The natural block is most efficient unit of image access for the underlying format.
    /// For many formats this is simple a whole scanline (1 x width_).
    gdal_dimension::type block_width;

    /// Number of columns of pixels in the natural block.
    gdal_dimension::type block_height;
};

/// Read settings for images accessed through GDAL.
///
/// The structure can be used for all read_xxx functions, except read_image_info.
template<>
struct image_write_info<gdal_tag>
{
    /// Default constructor
    image_write_info()
    {
    }

    // TODO
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_GDAL_TAGS_HPP
