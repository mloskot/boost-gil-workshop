#include <boost/gil.hpp>
#include <boost/mp11.hpp>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <vector>
#include "../utility.hpp"
using namespace boost;

using bgr121_ref_t = gil::bit_aligned_pixel_reference<
    std::uint8_t,
    mp11::mp_list_c<int, 1, 2, 1>,
    gil::bgr_layout_t,
    true
> const;
using bgr121_image_t = gil::image<bgr121_ref_t, false>;
using bgr121_view_t = typename bgr121_image_t::view_t;
using bgr121_value_t = typename bgr121_view_t::value_type;


int main()
{
    try
    {
        gil::rgb8_pixel_t red8(255, 0, 0), green8(0, 255, 0), blue8(0, 0, 255), white8(255, 255, 255);
        bgr121_value_t red, green, blue, white;

        gil::color_convert(red8, red);
        gil::default_color_converter()(red8, red);
        red = gil::color_convert_deref_fn<gil::rgb8_ref_t, bgr121_value_t>()(red8);
        gil::color_convert(green8, green);
        gil::color_convert(blue8, blue);
        gil::color_convert(white8, white);

        bgr121_image_t img(typename bgr121_view_t::point_t(20,20));
        bgr121_view_t const& img_view = view(img);
        auto d0 = dump(img_view);
        save(d0, "draw_d0");

        fill(img_view.begin(), img_view.end(), red);
        auto d1 = dump(img_view);
        save(d1, "draw_d1");

        gil::color_convert(red8, img_view[0]);
        auto d2 = dump(img_view);
        save(d2, "draw_d2");

        // draw a blue line along the diagonal
        typename bgr121_view_t::xy_locator loc = img_view.xy_at(0, img_view.height() - 1);
        for (int y = 0; y < img_view.height(); ++y)
        {
            *loc = blue;
            ++loc.x();
            loc.y()--;
        }
        auto d3 = dump(img_view);
        save(d3, "draw_d3");

        // draw a green dotted line along the main diagonal with step of 3
        loc = img_view.xy_at(img_view.width() - 1, img_view.height() - 1);
        while (loc.x() >= img_view.x_at(0, 0))
        {
            *loc = green;
            loc -= typename bgr121_view_t::point_t(3, 3);
        }
        auto d4 = dump(img_view);
        save(d4, "draw_d4");


        return 0;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;

    }
    return 0;
}
