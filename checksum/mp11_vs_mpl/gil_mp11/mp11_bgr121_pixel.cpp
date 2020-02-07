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
        auto p0 = sample_view[0];
        auto p00 = p0[0];

        return 0;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;

    }
    return 0;
}
