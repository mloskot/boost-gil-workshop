#include <boost/assert.hpp>
#include <boost/gil.hpp>
#include <cstdint>
#include <exception>
#include <iostream>
#include "../utility.hpp"
#include "../bgr121.hpp"
namespace gil = boost::gil;


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

void run(std::ptrdiff_t w, std::ptrdiff_t h, std::ptrdiff_t step)
{
    std::string const name = "stage0_" + std::to_string(w) + "x" + std::to_string(h) + "s" + std::to_string(step);

#ifdef USE_GLOBAL_COLORS
    initialize();
#else
    gil::rgb8_pixel_t red8(255, 0, 0), green8(0, 255, 0), blue8(0, 0, 255), white8(255, 255, 255);
    bgr121_value_t red(0), green(0), blue(0), white(0);
    gil::color_convert(red8, red);
    gil::default_color_converter()(red8, red);
    red = gil::color_convert_deref_fn<gil::rgb8_ref_t, bgr121_value_t>()(red8);
    gil::color_convert(green8, green);
    gil::color_convert(blue8, blue);
    gil::color_convert(white8, white);
#endif
    verify_colors();

    bgr121_image_t img(typename bgr121_view_t::point_t(w, h));
    bgr121_view_t const& img_view = view(img);

    //save_dump(view(img), name + "_dump0"); // uninitialized noise

    fill(img_view.begin(), img_view.end(), red);
    verify_colors();
    save_dump(view(img), name + "_dump1");

    // draw a blue line along the diagonal
    typename bgr121_view_t::xy_locator loc = img_view.xy_at(0, img_view.height() - 1);
    for (int y = 0; y < img_view.height(); ++y)
    {
        verify_colors();
        *loc = blue;
        ++loc.x();
        loc.y()--;
    }
    verify_colors();
    save_dump(view(img), name + "_dump2");

    // draw a green dotted line along the main diagonal with step of 3
    //{
    //    // BUG!
    //    // Re-using loc declared above leaks the bug
    //    loc = img_view.xy_at(img_view.width() - 1, img_view.height() - 1);
    //    // Defining loc fixes the bug
    //    //typename bgr121_view_t::xy_locator loc = img_view.xy_at(img_view.width() - 1, img_view.height() - 1);
    //    while (loc.x() >= img_view.x_at(0, 0))
    //    {
    //        *loc = green;
    //        // BUG:
    //        // for image 20,20
    //        // step 1,1 or 3,3 or 5,5 break in MSVC 64-bit release vs debug
    //        // step 2,2 or 4,4 or 6,6 give consistent results
    //        loc -= typename bgr121_view_t::point_t(step, step);
    //    }
    //    save_dump(view(img), name + "_dump3");
    //}
}

//#define DUMP_MINIMAL
int main()
{
    try
    {
#ifdef DUMP_MINIMAL
        run(20, 20, 2);
        run(20, 20, 3);
#else
        for (auto d : {10, 20})
        {
            for (auto s : {1, 2, 3, 4, 5, 6, 10})
                run(d, d, s);
        }
#endif

        return 0;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;

    }
    return 0;
}
