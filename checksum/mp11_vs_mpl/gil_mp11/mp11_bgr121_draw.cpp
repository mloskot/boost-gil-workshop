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

template <typename View>
void save_dump(View const& view, std::string filename)
{
    auto d0 = dump(view);
    save(d0, filename);
}

void run1(std::ptrdiff_t w, std::ptrdiff_t h, std::ptrdiff_t step)
{
    std::string const name = std::to_string(w) + "x" + std::to_string(h) + "s" + std::to_string(step) + "_run1";

    gil::rgb8_pixel_t red8(255, 0, 0), green8(0, 255, 0), blue8(0, 0, 255), white8(255, 255, 255);
    bgr121_value_t red, green, blue, white;

    gil::color_convert(red8, red);
    gil::default_color_converter()(red8, red);
    red = gil::color_convert_deref_fn<gil::rgb8_ref_t, bgr121_value_t>()(red8);
    gil::color_convert(green8, green);
    gil::color_convert(blue8, blue);
    gil::color_convert(white8, white);

    bgr121_image_t img(typename bgr121_view_t::point_t(w, h));
    bgr121_view_t const& img_view = view(img);
    save_dump(img_view, "draw_d0_" + name);

    fill(img_view.begin(), img_view.end(), red);
    save_dump(view(img), "draw_d1_" + name);

    gil::color_convert(red8, img_view[0]);
    save_dump(view(img), "draw_d2_" + name);

    // draw a blue line along the diagonal
    typename bgr121_view_t::xy_locator loc = img_view.xy_at(0, img_view.height() - 1);
    for (int y = 0; y < img_view.height(); ++y)
    {
        *loc = blue;
        ++loc.x();
        loc.y()--;
    }
    save_dump(view(img), "draw_d3_" + name);

    // draw a green dotted line along the main diagonal with step of 3
    loc = img_view.xy_at(img_view.width() - 1, img_view.height() - 1);
    while (loc.x() >= img_view.x_at(0, 0))
    {
        *loc = green;
        // BUG:
        // for image 20,20
        // step 1,1 or 3,3 or 5,5 break in MSVC 64-bit release vs debug
        // step 2,2 or 4,4 or 6,6 give consistent results
        loc -= typename bgr121_view_t::point_t(step, step);
    }
    save_dump(view(img), "draw_d4_" + name);
}

void run2(std::ptrdiff_t w, std::ptrdiff_t h, std::ptrdiff_t step)
{
    std::string const name = std::to_string(w) + "x" + std::to_string(h) + "s" + std::to_string(step) + "_run2";

    gil::rgb8_pixel_t red8(255, 0, 0), green8(0, 255, 0), blue8(0, 0, 255), white8(255, 255, 255);
    bgr121_value_t red, green, blue, white;

    gil::color_convert(red8, red);
    gil::default_color_converter()(red8, red);
    red = gil::color_convert_deref_fn<gil::rgb8_ref_t, bgr121_value_t>()(red8);
    gil::color_convert(green8, green);
    gil::color_convert(blue8, blue);
    gil::color_convert(white8, white);

    bgr121_image_t img(typename bgr121_view_t::point_t(w, h));
    save_dump(view(img), "draw_d0_" + name);
    {
        bgr121_view_t const& img_view = view(img);
        fill(img_view.begin(), img_view.end(), red);
        save_dump(view(img), "draw_d1_" + name);
    }
    {
        bgr121_view_t const& img_view = view(img);
        gil::color_convert(red8, img_view[0]);
        save_dump(view(img), "draw_d2_" + name);
    }
    {
        bgr121_view_t const& img_view = view(img);
        // draw a blue line along the diagonal
        typename bgr121_view_t::xy_locator loc = img_view.xy_at(0, img_view.height() - 1);
        for (int y = 0; y < img_view.height(); ++y)
        {
            *loc = blue;
            ++loc.x();
            loc.y()--;
        }
        save_dump(view(img), "draw_d3_" + name);
    }
    {
        bgr121_view_t const& img_view = view(img);
        // draw a green dotted line along the main diagonal with step of 3
        typename bgr121_view_t::xy_locator loc = img_view.xy_at(img_view.width() - 1, img_view.height() - 1);
        while (loc.x() >= img_view.x_at(0, 0))
        {
            *loc = green;
            // give consistent results all the time
            loc -= typename bgr121_view_t::point_t(step, step);
        }
        save_dump(view(img), "draw_d4_" + name);
    }
}

int main()
{
    try
    {
        for (auto d : {10, 20})
        {
            for (auto s : {1, 2, 3, 4, 5, 6, 10})
            {
                run1(d, d, s);
                run2(d, d, s);
            }
        }

        return 0;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;

    }
    return 0;
}
