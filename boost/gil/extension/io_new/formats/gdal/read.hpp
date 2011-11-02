//
// Copyright 2010-2011 (c) Mateusz Loskot <mateusz@loskot.net>
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
//
#ifndef BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_READ_HPP
#define BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_READ_HPP

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/bit_operations.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>

#include <boost/gil/extension/io_new/gdal_tags.hpp>

#include "device.hpp"
#include "is_allowed.hpp"

namespace boost { namespace gil { namespace detail {

template<typename Device, typename ConversionPolicy>
class reader<Device, gdal_tag, ConversionPolicy>
    : public reader_base<gdal_tag, ConversionPolicy>
{
public:

    reader(Device& device, image_read_settings<gdal_tag> const& settings)
        : reader_base<gdal_tag, ConversionPolicy>(settings)
        , io_device_(device)
    {}

    image_read_info<gdal_tag> get_info()
    {
        image_read_info<gdal_tag> info;
        info.width = io_device_.get_width();
        info.height = io_device_.get_height();
        info.bits_per_channel = io_device_.get_bits_per_channel();
        info.channels_per_pixel = io_device_.get_channels_per_pixel();
        
        std::pair<int, int> block_size = io_device_.get_block_size();
        info.block_width = block_size.first;
        info.block_height = block_size.second;
        
        return info;
    }

    template<typename View>
    void apply(View& dst_view)
    {
    }

protected:

    Device& io_device_;
    image_read_info<gdal_tag> info_; // TODO: what for?
};

struct gdal_read_is_supported
{
    template<typename View>
    struct apply : public is_read_supported<typename get_pixel_type<View>::type, gdal_tag> {};
};


template<typename Device>
class dynamic_image_reader<Device, gdal_tag>
    : public reader<Device, gdal_tag, detail::read_and_no_convert>
{
    typedef reader<Device, gdal_tag, detail::read_and_no_convert> base_reader_type;

public:

    dynamic_image_reader(Device& device, image_read_settings<gdal_tag> const& settings)
        : base_reader_type(device, settings)
    {}

    template<typename Images>
    void apply(any_image<Images>& images)
    {
        // TODO        
    }
};

}}} // namespace booost::gil::detail

#endif // BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_READ_HPP
