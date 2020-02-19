#include <boost/gil.hpp>
#include <cstdint>
#include <exception>
#include <iostream>
#include "../utility.hpp"
#include "../bgr121.hpp"
namespace gil = boost::gil;

void run(std::ptrdiff_t w, std::ptrdiff_t h, std::ptrdiff_t step)
{
    initialize();
    std::string const name = "stage0_" + std::to_string(w) + "x" + std::to_string(h) + "s" + std::to_string(step);

    bgr121_image_t img(typename bgr121_view_t::point_t(w, h));
    bgr121_view_t const& img_view = view(img);
    //save_dump(view(img), name + "_dump0"); // uninitialized noise

    fill(img_view.begin(), img_view.end(), red);
    //save_dump(view(img), name + "_dump1");

    gil::color_convert(red8, img_view[0]);
    //save_dump(view(img), name + "_dump2");

    // draw a blue line along the diagonal
    typename bgr121_view_t::xy_locator loc = img_view.xy_at(0, img_view.height() - 1);
    for (int y = 0; y < img_view.height(); ++y)
    {
        *loc = blue;
        ++loc.x();
        loc.y()--;
    }
    //save_dump(view(img), name + "_dump3");

    // draw a green dotted line along the main diagonal with step of 3
    {
        // BUG!
        // Re-using loc declared above leaks the bug
        loc = img_view.xy_at(img_view.width() - 1, img_view.height() - 1);
        // Defining loc fixes the bug
        //typename bgr121_view_t::xy_locator loc = img_view.xy_at(img_view.width() - 1, img_view.height() - 1);
        while (loc.x() >= img_view.x_at(0, 0))
        {
            *loc = green;
            // BUG:
            // for image 20,20
            // step 1,1 or 3,3 or 5,5 break in MSVC 64-bit release vs debug
            // step 2,2 or 4,4 or 6,6 give consistent results
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
