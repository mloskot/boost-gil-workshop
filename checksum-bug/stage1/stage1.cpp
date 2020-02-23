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

gil::rgb8_pixel_t red8(255, 0, 0), blue8(0, 0, 255);
bgr121_value_t red(0), blue(0);

template <typename View>
void save_dump(View const& v, std::string name)
{
    auto const f = "stage1_" + name + "_" + std::to_string(v.width()) + "x" + std::to_string(v.height());
    save(dump(v), fs::path(__FILE__).parent_path(), f);
}

void init()
{
    gil::color_convert(red8, red);
    gil::color_convert(blue8, blue);
}

void draw_loop(std::ptrdiff_t w, std::ptrdiff_t h)
{
    init();
    bgr121_image_t img(w, h);
    {
        auto v = view(img);
        fill(v.begin(), v.end(), red);
        save_dump(view(img), "dump1");
    }
    // draw a blue line along the diagonal
    {
        auto v = view(img);
        auto loc = v.xy_at(0, v.height() - 1);
        for (std::ptrdiff_t y = 0; y < v.height(); ++y)
        {
            *loc = blue;
            ++loc.x();
            --loc.y();
        }
        save_dump(view(img), "dump2_loop");
    }
}

void draw_step(std::ptrdiff_t w, std::ptrdiff_t h)
{
    init();
    bgr121_image_t img(w, h);
    {
        auto v = view(img);
        fill(v.begin(), v.end(), red);
        save_dump(view(img), "dump1");
    }
    // draw a blue line along the diagonal
    {
        auto v = view(img);
        auto loc = v.xy_at(0, v.height() - 1);
        *loc = blue; // red red blue
        ++loc.x();
        --loc.y();
        *loc = blue; // red blue red
        ++loc.x();
        --loc.y();
        *loc = blue; // blue red red
        ++loc.x();
        --loc.y();
        save_dump(view(img), "dump2_step");
    }
}

int main()
{
    try
    {
        draw_loop(3, 3);
        draw_step(3, 3);

        return EXIT_SUCCESS;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;

    }
    return EXIT_FAILURE;
}
