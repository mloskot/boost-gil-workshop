#pragma once
#include <boost/gil.hpp>
#include <boost/mp11.hpp>
#include <cstdint>
namespace gil = boost::gil;

using bgr121_ref_t = gil::bit_aligned_pixel_reference<
    std::uint8_t,
    boost::mp11::mp_list_c<int, 1, 2, 1>,
    gil::bgr_layout_t,
    true
> const;

using bgr121_image_t = gil::image<bgr121_ref_t, false>;
using bgr121_view_t = typename bgr121_image_t::view_t;
using bgr121_value_t = typename bgr121_view_t::value_type;

gil::rgb8_pixel_t red8(255, 0, 0), green8(0, 255, 0), blue8(0, 0, 255), white8(255, 255, 255);
bgr121_value_t red(0), green(0), blue(0), white(0);

void initialize()
{

    gil::color_convert(red8, red);
    gil::default_color_converter()(red8, red);
    red = gil::color_convert_deref_fn<gil::rgb8_ref_t, bgr121_value_t>()(red8);
    gil::color_convert(green8, green);
    gil::color_convert(blue8, blue);
    gil::color_convert(white8, white);
}
