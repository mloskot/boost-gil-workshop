//
// Copyright 2010 (c) Mateusz Loskot <mateusz@loskot.net>
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
//
#ifndef BOOST_GIL_EXTENSION_IO_GDAL_TAGS_HPP
#define BOOST_GIL_EXTENSION_IO_GDAL_TAGS_HPP

extern "C" {
    #include <gdal.h> // or rather C++ API?
}

#include "detail/base.hpp"

namespace boost { namespace gil {

/// Defines tag for images accessed using GDAL.
struct gdal_tag : format_tag {};

/// For a description see: http://www.gdal.org

/// Defines type for image width property.
struct gdal_image_width : property_base<int> {};

/// Defines type for image height property.
struct gdal_image_height : property_base<int> {};

/// Defines type for bits per sample property.
struct gdal_bits_per_sample : property_base<int> {};

/// Defines type for samples per pixel property.
struct gdal_samples_per_pixel : property_base<int> {};

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
    typedef int dimension_type;

    image_read_info()
        : width_(gdal_image_width::type())
        , height_(gdal_image_height::type())
        , bits_per_sample_(gdal_bits_per_sample::type())
        , samples_per_pixel_(gdal_samples_per_pixel::type())
    {}

    /// The number of rows of pixels in the image.
    gdal_image_height::type width_;

    /// The number of columns in the image, i.e., the number of pixels per row.
    gdal_image_height::type height_;

    /// Size of image sample.
    gdal_bits_per_sample::type bits_per_sample_;

    /// Size of image pixel.
    gdal_samples_per_pixel::type samples_per_pixel_;
};

/// Read settings for images accessed through GDAL.
///
/// The structure can be used for all read_xxx functions, except read_image_info.
template<>
struct image_write_info<gdal_tag>
{
    /// Default constructor
    image_write_info() {}

    // TODO
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_GDAL_TAGS_HPP
