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
extern gil::rgb8c_planar_view_t sample_view;

using bgr121_ref_t = gil::bit_aligned_pixel_reference<
    std::uint8_t,
    mp11::mp_list_c<int, 1, 2, 1>,
    gil::bgr_layout_t,
    true
> const;
using bgr121_image_t = gil::image<bgr121_ref_t, false>;

int main()
{
    try
    {
        auto d0 = dump(sample_view);
        save(d0, "d0.txt");

        bgr121_image_t img;
        img.recreate(sample_view.dimensions());
        gil::copy_and_convert_pixels(sample_view, view(img));
        auto d1 = dump(const_view(img));
        save(d1, "d1.txt");

        gil::rgb8_image_t rgb_img(view(img).dimensions());
        gil::copy_and_convert_pixels(view(img), view(rgb_img));
        auto d2 = dump(const_view(rgb_img));

        return 0;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;

    }
    return 0;
}
