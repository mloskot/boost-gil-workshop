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
public:
    typedef ::boost::shared_ptr<GDALDataset> gdal_ds_type;

    gdal_device_base() {}
    gdal_device_base(GDALDataset* gdal_ds) : gdal_ds_(gdal_ds, ::GDALClose) {}
    gdal_device_base(gdal_ds_type gdal_ds) : gdal_ds_(gdal_ds) {}

    int get_width() const
    {
        return gdal_ds_->GetRasterXSize();
    }

    int get_height() const
    {
        return gdal_ds_->GetRasterYSize();
    }

    int get_bits_per_channel() const
    {
        // Read bits depth of first channel, assume homogeneous channels

        // GDAL indexes bands from 1
        GDALDataType band_type = get_band(1).GetRasterDataType();
        assert(GDT_Unknown != band_type);
        return GDALGetDataTypeSize(band_type);
    }
    
    int get_channels_per_pixel() const
    {
        return gdal_ds_->GetRasterCount();
    }

    std::pair<int, int> get_block_size() const
    {
        // Read bits depth of first channel, assume homogeneous channels
        std::pair<int, int> size;
        get_band(1).GetBlockSize(&size.first, &size.second);
        return size;
    }

protected:
    gdal_ds_type gdal_ds_;
    
    GDALRasterBand& get_band(int index) const
    {
        // GDAL indexes bands from 1
        GDALRasterBand* band = gdal_ds_->GetRasterBand(index);
        io_error_if(!band, "band access failure");
        return *band;
    }

}; // gdal_device_base

// TODO: gdal_io_error type
void gdal_io_error_if_last(bool expr)
{
    if (!expr && CE_None != CPLGetLastErrorType())
    {
        char const* msg = CPLGetLastErrorMsg();
        std::ostringstream oss;
        oss << " [" << CPLGetLastErrorNo() << "] " << (msg ? msg : "unknown");

        io_error(oss.str().c_str());
    }
}

struct gdal_ds_deleter
{
    void operator()(GDALDataset* gdal_ds)
    {
        if (gdal_ds)
        {
            ::GDALClose(gdal_ds);
            gdal_ds = 0;
        }
    }
};

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
            
        GDALAllRegister(); // TODO: find better place to call it once only

        gdal_ds_type gdal_ds(static_cast<GDALDataset*>(
            ::GDALOpen(name.c_str(), access)), gdal_ds_deleter());

        gdal_io_error_if_last(0 != gdal_ds.get());
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
