#include <algorithm>
#include <iostream>
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/tiff_all.hpp>
using std::cout;
using std::endl;
using namespace boost::gil;

int main()
{
    typedef bit_aligned_image5_type< 16, 16, 16, 8, 8, devicen_layout_t<5> >::type image_t;
    image_t img;
    read_image("g:\\tmp\\Shadow_cmyk8.tif", img, tiff_tag());

    return 0;
}
