#include <boost/gil.hpp>
#include <cstdint>
#include <exception>
#include <iostream>
#include "../utility.hpp"
#include "../bgr121.hpp"
namespace gil = boost::gil;

void run(std::ptrdiff_t w, std::ptrdiff_t h, std::ptrdiff_t step)
{
    std::string const name = "stage1_" + std::to_string(w) + "x" + std::to_string(h) + "s" + std::to_string(step);

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

    bgr121_image_t img(typename bgr121_view_t::point_t(w, h));
    bgr121_view_t const& img_view = view(img);
    //save_dump(view(img), name + "_dump0"); // uninitialized noise

    fill(img_view.begin(), img_view.end(), red);
    save_dump(view(img), name + "_dump1");

    gil::color_convert(red8, img_view[0]);
    save_dump(view(img), name + "_dump2");

    // draw a blue line along the diagonal
    typename bgr121_view_t::xy_locator loc = img_view.xy_at(0, img_view.height() - 1);
    for (int y = 0; y < img_view.height(); ++y)
    {
        *loc = blue;
        ++loc.x();
        loc.y()--;
    }
    save_dump(view(img), name + "_dump3");

    // draw a green dotted line along the main diagonal with step of 3
    {
        // FIX!
        // Re-using loc declared above leaks the bug
        // Defining loc fixes the bug
        typename bgr121_view_t::xy_locator loc = img_view.xy_at(img_view.width() - 1, img_view.height() - 1);
        while (loc.x() >= img_view.x_at(0, 0))
        {
            *loc = green;
            // give consistent results all the time
            loc -= typename bgr121_view_t::point_t(step, step);
        }
        save_dump(view(img), name + "_dump4");
    }
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
