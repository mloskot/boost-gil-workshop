#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#pragma warning(disable:4244)
#pragma warning(disable:4503)
#pragma warning(disable:4996)
#include <iostream>
#include <typeinfo>
#include <boost/gil.hpp>
namespace gil = boost::gil;

using bit_aligned_pixel_bgr232_refefence = gil::bit_aligned_pixel_reference
    <
        std::uint8_t,
        boost::mp11::mp_list_c<int, 2, 3, 2>,
        gil::bgr_layout_t,
        true
    > const;

using bit_aligned_pixel_bgr232_iterator = gil::bit_aligned_pixel_iterator<bit_aligned_pixel_bgr232_refefence>;

using bit_aligned_pixel_bgr232 = std::iterator_traits<bit_aligned_pixel_bgr232_iterator>::value_type;

int main()
{
    //bit_aligned_pixel_bgr232 x;

    gil::point_t dims{32, 32};
    gil::bgr8_pixel_t p{32, 64, 128};
    gil::bgr8_image_t i(dims, p);
    
    return 0;
}
