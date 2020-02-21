#pragma once
#include <boost/assert.hpp>
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

#if 0
#ifdef USE_GLOBAL_COLORS
gil::rgb8_pixel_t red8(255, 0, 0), green8(0, 255, 0), blue8(0, 0, 255);
bgr121_value_t red(0), green(0), blue(0);

gil::rgb8_pixel_t const red8_check(255, 0, 0);
gil::rgb8_pixel_t const green8_check(0, 255, 0);
gil::rgb8_pixel_t const blue8_check(0, 0, 255);
bgr121_value_t const red_check(8);
bgr121_value_t const green_check(6);
bgr121_value_t const blue_check(1);

void initialize()
{
    gil::color_convert(red8, red);
    gil::default_color_converter()(red8, red);
    red = gil::color_convert_deref_fn<gil::rgb8_ref_t, bgr121_value_t>()(red8);
    gil::color_convert(green8, green);
    gil::color_convert(blue8, blue);
}
#endif

void verify_colors()
{
    if (red8 != red8_check) throw std::runtime_error("red8 broke");
    if (green8 != green8_check) throw std::runtime_error("green8 broke");
    if (blue8 != blue8_check) throw std::runtime_error("blue8 broke");
    if (red != red_check) throw std::runtime_error("red broke");
    if (green != green_check) throw std::runtime_error("green broke");
    if (blue != blue_check) throw std::runtime_error("blue broke");
}

#endif
