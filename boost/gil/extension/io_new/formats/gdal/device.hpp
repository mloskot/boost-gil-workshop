//
// Copyright 2011 (c) Mateusz Loskot <mateusz@loskot.net>
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
//
#ifndef BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_DEVICE_HPP
#define BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_DEVICE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

#include <gdal_priv.h> // GDAL C++ API

namespace boost { namespace gil { namespace detail {

class gdal_device_base
{
protected:
    
    typedef shared_ptr<::GDALDataset> gdal_ds_type;

public:
    gdal_device_base() {}
    gdal_device_base(GDALDataset* gdal_ds) : gdal_ds_(gdal_ds, ::GDALClose) {}
    gdal_device_base(gdal_ds_type gdal_ds) : gdal_ds_(gdal_ds) {}

    std::size_t get_width() const
    {
        return std::size_t(gdal_ds_->GetRasterXSize());
    }

    std::size_t get_height() const
    {
        return std::size_t(gdal_ds_->GetRasterYSize());
    }

    std::size_t get_num_channels() const
    {
        return std::size_t(gdal_ds_->GetRasterCount());
    }

protected:
    gdal_ds_type gdal_ds_;

}; // gdal_device_base

// TODO: gdal_io_error type
void gdal_io_error_if_last(bool expr)
{
    if (expr && CE_None != ::CPLGetLastErrorType())
    {
        char const* msg = ::CPLGetLastErrorMsg();
        std::ostringstream oss;
        oss << " [" << ::CPLGetLastErrorNo() << "] " << (msg ? msg : "unknown");

        io_error(oss.str().c_str());
    }
}

template<>
struct file_stream_device<gdal_tag> : public gdal_device_base
{
    struct read_tag {};
    struct write_tag {};

    file_stream_device(std::string const& name, read_tag)
        : gdal_device_base(open(name, ::GA_ReadOnly))
    {
    }

    file_stream_device(std::string const& name, write_tag)
        : gdal_device_base(open(name, ::GA_Update))
    {
    }

    file_stream_device(GDALDataset* gdal_ds) : gdal_device_base(gdal_ds) {}

    static gdal_ds_type open(std::string const& name, ::GDALAccess access)
    {
        gdal_ds_type gdal_ds(static_cast<::GDALDataset*>(
            ::GDALOpen(name.c_str(), access)), ::GDALClose);

        gdal_io_error_if_last(gdal_ds);
        return gdal_ds;
    }

}; // file_stream_device

//
// template< typename T, typename D >
// struct is_adaptable_input_device< tiff_tag, T, D > : mpl::false_{};
//

template<typename FormatTag>
struct is_adaptable_input_device<FormatTag, GDALDataset*, void> : mpl::true_
{
    typedef file_stream_device<FormatTag> device_type;
};

template<typename FormatTag>
struct is_adaptable_output_device<FormatTag, GDALDataset*, void> : mpl::true_
{
    typedef file_stream_device<FormatTag> device_type;
};

}}} // namespace boost::gil::detail

#endif // BOOST_GIL_EXTENSION_IO_FORMATS_GDAL_DEVICE_HPP
