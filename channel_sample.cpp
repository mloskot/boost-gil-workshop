#include <boost/gil.hpp>
namespace gil = boost::gil;

int main()
{
    using rgb565_pixel_t = gil::packed_pixel_type
        <
            std::uint16_t,
            boost::mpl::vector3_c<unsigned,5,6,5>,
            gil::rgb_layout_t
        >::type;
    boost::function_requires<gil::PixelValueConcept<rgb565_pixel_t>>();

    rgb565_pixel_t p;
    gil::get_color(p, gil::green_t()) = gil::get_color(p, gil::blue_t());

}
