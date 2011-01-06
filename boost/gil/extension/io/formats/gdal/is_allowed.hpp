//
// Copyright 2010 (c) Mateusz Loskot <mateusz@loskot.net>
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
//
#ifndef BOOST_GIL_EXTENSION_IO_GDAL_IO_IS_ALLOWED_HPP
#define BOOST_GIL_EXTENSION_IO_GDAL_IO_IS_ALLOWED_HPP

namespace boost { namespace gil { namespace detail {

template<typename View>
bool is_allowed(image_read_info<gdal_tag> const& info,
    mpl::true_  // is read_and_no_convert
    )
{
    // TODO
    return true;
}

template<typename View>
bool is_allowed(image_read_info<gdal_tag> const& info,
    mpl::false_  // is read_and_convert
    )
{
    // TODO
    return true;
}

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_GDAL_IO_IS_ALLOWED_HPP
