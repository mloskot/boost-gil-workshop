//
// Copyright 2010-2011 (c) Mateusz Loskot <mateusz@loskot.net>
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
//
#ifndef BOOST_GIL_EXTENSION_IO_GDAL_SUPPORTED_TYPES_HPP
#define BOOST_GIL_EXTENSION_IO_GDAL_SUPPORTED_TYPES_HPP

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/gil/channel.hpp>
#include <boost/gil/color_base.hpp>

namespace boost{ namespace gil { namespace detail {

// GDAL supports more than hundred of raster formats, so assume
// it can read and write virtually supports everything.

// Read support

struct gdal_read_support : read_support_true {};

// Write support

//struct gdal_write_support : write_support_true {};

} // namespace detail

template<typename Pixel>
struct is_read_supported
    <
    Pixel,
    gdal_tag
    > 
    : mpl::bool_<detail::gdal_read_support::is_supported>
{};

//template<typename Pixel>
//struct is_write_supported
//    <
//    Pixel,
//    gdal_tag
//    > 
//    : mpl::bool_<detail::gdal_write_support::is_supported>
//{};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_GDAL_SUPPORTED_TYPES_HPP

