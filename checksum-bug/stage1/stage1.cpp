#include <boost/gil.hpp>
#include <cstdint>
#include <exception>
#include <iostream>
#include "../utility.hpp"
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

void test(std::ptrdiff_t w, std::ptrdiff_t h)
{
    std::string const name = "stage0_" + std::to_string(w) + "x" + std::to_string(h);

    gil::rgb8_pixel_t red8(255, 0, 0), blue8(0, 0, 255);
    bgr121_value_t red(0), blue(0);
    gil::color_convert(red8, red);
    gil::color_convert(blue8, blue);

    bgr121_image_t img(w, h);
    {
        auto v = view(img);
        fill(v.begin(), v.end(), red);
        save_dump(view(img), name + "_dump1");
    }
    {
        // draw a blue line along the diagonal
        auto const h = view(img).height();
        auto loc = view(img).xy_at(0, h - 1);
        for (std::ptrdiff_t y = 0; y < h; ++y)
        {
            *loc = blue;
            ++loc.x();
            --loc.y();
        }
        save_dump(view(img), name + "_dump2");
    }
}
int main()
{
    try
    {
        test(3, 3);
        test(10, 10);
        test(20, 20);

        return EXIT_SUCCESS;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;

    }
    return EXIT_FAILURE;
}
