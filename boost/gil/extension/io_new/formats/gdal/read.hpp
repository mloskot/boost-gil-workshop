//
// Copyright 2010 (c) Mateusz Loskot <mateusz@loskot.net>
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
//
#ifndef BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_READ_HPP
#define BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_READ_HPP

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/bit_operations.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>

// TODO: GDAL lives in its own repo, uses gil/extension/io/ path
#include <boost/gil/extension/io_new/gdal_tags.hpp>

#include "is_allowed.hpp"

namespace boost { namespace gil { namespace detail {

template
    <
    typename Device,
    typename ConversionPolicy
    >
class reader<Device, gdal_tag, ConversionPolicy>
    : public reader_base<gdal_tag, ConversionPolicy>
{
public:

    reader(Device&device, image_read_settings<gdal_tag> const& settings)
        : reader_base<gdal_tag, ConversionPolicy>(settings)
        , io_device_(device)
    {}

protected:

    Device& io_device_;
    image_read_info<gdal_tag> info_;
};

#endif // BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_READ_HPP
