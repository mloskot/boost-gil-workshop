//
// Copyright 2010-2011 (c) Mateusz Loskot <mateusz@loskot.net>
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
//
#ifndef BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_READ_HPP
#define BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_READ_HPP

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/bit_operations.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>

#include <boost/gil/extension/io_new/gdal_tags.hpp>
#include <boost/gil/extension/io_new/formats/gdal/device.hpp>
#include <boost/gil/extension/io_new/formats/gdal/is_allowed.hpp>

namespace boost
{

namespace gil
{

namespace detail
{

template<typename Device, typename ConversionPolicy>

class reader<Device, gdal_tag, ConversionPolicy>
            : public reader_base<gdal_tag, ConversionPolicy>
{
    typedef reader_base<gdal_tag, ConversionPolicy> base;
    typedef image_read_settings<gdal_tag> settings_type;

public:

    reader(Device& device, image_read_settings<gdal_tag> const& settings)
            : reader_base<gdal_tag, ConversionPolicy>(settings)
            , io_device_(device)
    {
    }

    image_read_info<gdal_tag> get_info()
    {
        info_.width_ = io_device_.get_width();
        info_.height_ = io_device_.get_height();
        info_.bits_per_channel = io_device_.get_bits_per_channel();
        info_.channels_per_pixel = io_device_.get_channels_per_pixel();

        std::pair<int, int> block_size = io_device_.get_block_size();
        info_.block_width = block_size.first;
        info_.block_height = block_size.second;

        return info_;
    }

    template<typename View>
    void apply(View& dst_view)
    {
        read_data<row_buffer_helper_view<gray8_view_t> >(dst_view);
    }

private:

    template<typename Buffer, typename View>
    void read_data(View const& dst_view)
    {
        typedef Buffer row_buffer_helper_t;
        typedef typename row_buffer_helper_t::buffer_t buffer_t;

        std::size_t size_to_allocate = dst_view.width() * dst_view.height();
        row_buffer_helper_t row_buffer_helper(size_to_allocate, true);

        settings_type const& settings = base::_settings;
            
        typedef typename row_buffer_helper_t::iterator_t iterator_t;
        iterator_t begin = row_buffer_helper.begin();
        iterator_t first = begin + settings._top_left.x;
        iterator_t last  = first + settings._dim.x; // one after last element
            
        std::ptrdiff_t const row_end = settings._top_left.y + settings._dim.y;
        std::ptrdiff_t dst_row = 0;

        for (std::ptrdiff_t row = settings._top_left.y; row < row_end; ++row, ++dst_row)
        {
            io_device_.read_scanline(row_buffer_helper.buffer(), row);

            base::_cc_policy.read(first, last, dst_view.row_begin(dst_row));
        }
    }

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

}
}
} // namespace booost::gil::detail

#endif // BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_READ_HPP
