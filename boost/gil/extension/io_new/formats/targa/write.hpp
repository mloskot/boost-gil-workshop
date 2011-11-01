/*
    Copyright 2010 Kenneth Riddile
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_TARGA_IO_WRITE_HPP
#define BOOST_GIL_EXTENSION_IO_TARGA_IO_WRITE_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief
/// \author Kenneth Riddile \n
///
/// \date 2010 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <boost/gil/extension/io_new/detail/base.hpp>
#include <boost/gil/extension/io_new/detail/io_device.hpp>

#include <boost/gil/extension/io_new/targa_tags.hpp>

namespace boost { namespace gil { namespace detail {

template < int N > struct get_targa_view_type {};
template <> struct get_targa_view_type< 3 > { typedef bgr8_view_t type; };
template <> struct get_targa_view_type< 4 > { typedef bgra8_view_t type; };

template< typename Device >
class writer< Device
            , targa_tag
            >
{
public:

    writer( Device& file )
    : _out( file )
    {
    }

    ~writer()
    {
    }

    template<typename View>
    void apply( const View& view )
    {
        write( view );
    }

    template<typename View>
    void apply( const View&                           view
              , const image_write_info< targa_tag >& /* info */
              )
    {
        // Add code here, once image_write_info< targa_tag > isn't empty anymore.

        write( view );
    }

private:

    template< typename View >
    void write( const View& view )
    {
        uint8_t bit_depth = static_cast<uint8_t>( num_channels<View>::value * 8 );

        // write the TGA header
        _out.write_uint8( 0 ); // offset
        _out.write_uint8( targa_color_map_type::_rgb );
        _out.write_uint8( targa_image_type::_rgb );
        _out.write_uint16( 0 ); // color map start
        _out.write_uint16( 0 ); // color map length
        _out.write_uint8( 0 ); // color map depth
        _out.write_uint16( 0 ); // x origin
        _out.write_uint16( 0 ); // y origin
        _out.write_uint16( static_cast<uint16_t>( view.width() ) ); // width in pixels
        _out.write_uint16( static_cast<uint16_t>( view.height() ) ); // height in pixels
        _out.write_uint8( bit_depth );

        if( 32 == bit_depth )
        {
            _out.write_uint8( 8 ); // 8-bit alpha channel descriptor
        }
        else
        {
            _out.write_uint8( 0 );
        }

        write_image< View
                   , typename get_targa_view_type< num_channels< View >::value >::type
                   >( view );
    }


    template< typename View
            , typename TGA_View
            >
    void write_image( const View& view )
    {
        size_t row_size = view.width() * num_channels<View>::value;
        byte_vector_t buffer( row_size );
        std::fill( buffer.begin(), buffer.end(), 0 );


        TGA_View row = interleaved_view( view.width()
                                       , 1
                                       , reinterpret_cast<typename TGA_View::value_type*>( &buffer.front() )
                                       , row_size
                                       );

        for( typename View::y_coord_t y = view.height() - 1; y > -1; --y )
        {
            copy_pixels( subimage_view( view
                                      , 0
                                      , static_cast<int>( y )
                                      , static_cast<int>( view.width() )
                                      , 1
                                      )
                       , row
                       );

            _out.write( &buffer.front(), row_size );
        }

    }

private:

    Device& _out;
};


struct targa_write_is_supported
{
    template< typename View >
    struct apply
        : public is_write_supported< typename get_pixel_type< View >::type
                                   , targa_tag
                                   >
    {};
};

template< typename Device >
class dynamic_image_writer< Device
                          , targa_tag
                          >
    : public writer< Device
                   , targa_tag
                   >
{
    typedef writer< Device
                  , targa_tag
                  > parent_t;

public:

    dynamic_image_writer( Device& file )
    : parent_t( file )
    {}

    template< typename Views >
    void apply( const any_image_view< Views >& views )
    {
        dynamic_io_fnobj< targa_write_is_supported
                        , parent_t
                        > op( this );

        apply_operation( views, op );
    }
};

} // detail
} // gil
} // boost

#endif // BOOST_GIL_EXTENSION_IO_TARGA_IO_WRITE_HPP
