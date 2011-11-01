/*
    Copyright 2010 Kenneth Riddile
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_TARGA_IO_READ_HPP
#define BOOST_GIL_EXTENSION_IO_TARGA_IO_READ_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Kenneth Riddile \n
///
/// \date 2010 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/gil/extension/io_new/targa_tags.hpp>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/bit_operations.hpp>
#include <boost/gil/extension/io_new/detail/conversion_policies.hpp>
#include <boost/gil/extension/io_new/detail/row_buffer_helper.hpp>
#include <boost/gil/extension/io_new/detail/reader_base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>
#include <boost/gil/extension/io_new/detail/typedefs.hpp>

#include "is_allowed.hpp"

namespace boost { namespace gil { namespace detail {

template< typename Device
        , typename ConversionPolicy
        >
class reader< Device
            , targa_tag
            , ConversionPolicy
            >
    : public reader_base< targa_tag
                        , ConversionPolicy
                        >
{
private:

    typedef typename ConversionPolicy::color_converter_type cc_t;

public:

    reader( Device&                               device
          , const image_read_settings< targa_tag >& settings
          )
    : reader_base< targa_tag
                 , ConversionPolicy >( settings )
    , _io_dev( device )
    {}

    reader( Device&                               device
          , const cc_t&                           cc
          , const image_read_settings< targa_tag >& settings
          )
    : reader_base< targa_tag
                 , ConversionPolicy
                 >( cc
                  , settings
                  )
      , _io_dev( device )
    {}

    image_read_info< targa_tag > get_info()
    {
        _info._header_size = targa_header_size::_size;
        
        _info._offset = _io_dev.read_uint8() + _info._header_size;
        
        _info._color_map_type = _io_dev.read_uint8();
        if( _info._color_map_type != targa_color_map_type::_rgb )
        {
            io_error( "Indexed targa files are not supported" );
        }
        
        _info._image_type = _io_dev.read_uint8();
        if( _info._image_type != targa_image_type::_rgb &&
            _info._image_type != targa_image_type::_rle_rgb )
        {
            io_error( "Unsupported image type for targa file" );
        }
        
        _info._color_map_start = _io_dev.read_uint16();
        _info._color_map_length = _io_dev.read_uint16();
        _info._color_map_depth = _io_dev.read_uint8();
        
        _info._x_origin = _io_dev.read_uint16();
        _info._y_origin = _io_dev.read_uint16();
        
        _info._width = _io_dev.read_uint16();
        _info._height = _io_dev.read_uint16();
        if( _info._width < 1 || _info._height < 1 )
        {
            io_error( "Invalid dimension for targa file" );
        }
        
        _info._bits_per_pixel = _io_dev.read_uint8();
        if( _info._bits_per_pixel != 24 && _info._bits_per_pixel != 32 )
        {
            io_error( "Unsupported bit depth for targa file" );
        }
        
        _info._descriptor = _io_dev.read_uint8();
        if( (_info._bits_per_pixel == 24 && _info._descriptor != 0) ||
            (_info._bits_per_pixel == 32 && _info._descriptor != 8) )
        {
            io_error( "Unsupported descriptor for targa file" );
        }
        
        _info._valid = true;
        
        return _info;
    }

    template< typename View >
    void apply( const View& dst_view )
    {
        if( !_info._valid )
        {
            get_info();
        }

        typedef typename is_same< ConversionPolicy, read_and_no_convert >::type is_read_and_convert_t;
        io_error_if( !is_allowed< View >( _info, is_read_and_convert_t() ),
                     "Image types aren't compatible." );
        
        std::ptrdiff_t yend = _settings._dim.y;
        
        switch( _info._image_type )
        {
            case targa_image_type::_rgb:
            {
                if( _info._color_map_type != targa_color_map_type::_rgb )
                {
                    io_error( "Inconsistent color map type and image type in targa file." );
                }
                
                if( _info._color_map_length != 0 )
                {
                    io_error( "Non-indexed targa files containing a palette are not supported." );
                }
                
                switch( _info._bits_per_pixel )
                {
                    case 24:
                    {
                        read_data< bgr8_view_t >( dst_view );
                        break;
                    }
                    case 32:
                    {
                        read_data< bgra8_view_t >( dst_view );
                        break;
                    }
                    default:
                    {
                        io_error( "Unsupported bit depth in targa file." );
                        break;
                    }
                }
                
                break;
            }
            case targa_image_type::_rle_rgb:
            {
                if( _info._color_map_type != targa_color_map_type::_rgb )
                {
                    io_error( "Inconsistent color map type and image type in targa file." );
                }
                
                if( _info._color_map_length != 0 )
                {
                    io_error( "Non-indexed targa files containing a palette are not supported." );
                }
                
                switch( _info._bits_per_pixel )
                {
                    case 24:
                    {
                        read_rle_data< bgr8_view_t >( dst_view );
                        break;
                    }
                    case 32:
                    {
                        read_rle_data< bgra8_view_t >( dst_view );
                        break;
                    }
                    default:
                    {
                        io_error( "Unsupported bit depth in targa file." );
                        break;
                    }
                }
                
                break;
            }
            default:
            {
                io_error( "Unsupported image type in targa file." );
                break;
            }
        }
    }

private:

    // 8-8-8 BGR
    // 8-8-8-8 BGRA
    template< typename View_Src, typename View_Dst >
    void read_data( const View_Dst& view )
    {
        byte_vector_t row( _info._width * (_info._bits_per_pixel / 8) );

        // jump to first scanline
        _io_dev.seek( static_cast<long>(_info._offset) );

        View_Src v = interleaved_view( _info._width,
                                       1,
                                       reinterpret_cast<typename View_Src::value_type*>( &row.front() ),
                                       _info._width * num_channels< View_Src >::value );

        typename View_Src::x_iterator beg = v.row_begin( 0 ) + _settings._top_left.x;
        typename View_Src::x_iterator end = beg + _settings._dim.x;

        // read bottom up since targa origin is bottom left
        for( std::ptrdiff_t y = _settings._dim.y - 1; y > -1; --y )
        {
            // @todo: For now we're reading the whole scanline which is
            // slightly inefficient. Later versions should try to read
            // only the bytes which are necessary.
            _io_dev.read( &row.front(), row.size() );
            _cc_policy.read( beg, end, view.row_begin(y) );
        }
    }

    // 8-8-8 BGR
    // 8-8-8-8 BGRA
    template< typename View_Src, typename View_Dst >
    void read_rle_data( const View_Dst& view )
    {
        targa_depth::type bytes_per_pixel = _info._bits_per_pixel / 8;
        size_t image_size = _info._width * _info._height * bytes_per_pixel;
        byte_vector_t image_data( image_size );
        
        _io_dev.seek( static_cast<long>(_info._offset) );
        
        for( size_t pixel = 0; pixel < image_size; )
        {
            targa_offset::type current_byte = _io_dev.read_uint8();
            
            if( current_byte & 0x80 ) // run length chunk (high bit = 1)
            {
                uint8_t chunk_length = current_byte - 127;
                uint8_t pixel_data[4];
                for( size_t channel = 0; channel < bytes_per_pixel; ++channel )
                {
                    pixel_data[channel] = _io_dev.read_uint8();
                }
                
                // Repeat the next pixel chunk_length times
                for( uint8_t i = 0; i < chunk_length; ++i, pixel += bytes_per_pixel )
                {
                    memcpy( &image_data[pixel], pixel_data, bytes_per_pixel );
                }
            }
            else // raw chunk
            {
                uint8_t chunk_length = current_byte + 1;
                
                // Write the next chunk_length pixels directly
                size_t pixels_written = chunk_length * bytes_per_pixel;
                _io_dev.read( &image_data[pixel], pixels_written );
                pixel += pixels_written;
            }
        }

        View_Src v = flipped_up_down_view( interleaved_view( _info._width,
                                                             _info._height,
                                                             reinterpret_cast<typename View_Src::value_type*>( &image_data.front() ),
                                                             _info._width * num_channels< View_Src >::value ) );

        for( std::ptrdiff_t y = 0; y != _settings._dim.y; ++y )
        {
            typename View_Src::x_iterator beg = v.row_begin( y ) + _settings._top_left.x;
            typename View_Src::x_iterator end = beg + _settings._dim.x;
            _cc_policy.read( beg, end, view.row_begin(y) );
        }
    }

protected:

    Device& _io_dev;
    image_read_info< targa_tag > _info;
};

/////////////////////////////////// dynamic image

class targa_type_format_checker
{
public:

    targa_type_format_checker( const targa_depth::type& bpp )
    : _bpp( bpp )
    {}

    template< typename Image >
    bool apply()
    {
        if( _bpp < 32 )
        {
            return pixels_are_compatible< typename Image::value_type, rgb8_pixel_t >::value
                   ? true
                   : false;
        }
        else
        {
            return pixels_are_compatible< typename Image::value_type, rgba8_pixel_t >::value
                   ? true
                   : false;
        }
    }

private:

    const targa_depth::type& _bpp;
};

struct targa_read_is_supported
{
    template< typename View >
    struct apply : public is_read_supported< typename get_pixel_type< View >::type
                                           , targa_tag
                                           >
    {};
};

template< typename Device
        >
class dynamic_image_reader< Device
                          , targa_tag
                          >
    : public reader< Device
                   , targa_tag
                   , detail::read_and_no_convert
                   >
{
    typedef reader< Device
                  , targa_tag
                  , detail::read_and_no_convert
                  > parent_t;

public:

    dynamic_image_reader( Device&                               device
                        , const image_read_settings< targa_tag >& settings
                        )
    : parent_t( device
              , settings
              )
    {}

    template< typename Images >
    void apply( any_image< Images >& images )
    {
        if( !_info._valid )
        {
            parent_t::get_info();
        }

        targa_type_format_checker format_checker( _info._bits_per_pixel );

        if( !construct_matched( images
                              , format_checker
                              ))
        {
            io_error( "No matching image type between those of the given any_image and that of the file" );
        }
        else
        {
            init_image( images, _info );

            dynamic_io_fnobj< targa_read_is_supported
                            , parent_t
                            > op( this );

            apply_operation( view( images )
                           , op
                           );
        }
    }
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_TARGA_IO_READ_HPP
