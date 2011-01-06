#include <array>
#include <cassert>

#include <boost/gil/gil_all.hpp>
#include <gil/extension/io/gdal_read.hpp>

namespace bgil = boost::gil;

int main()
{

    std::string filename("image.tif");
    bgil::rgb8_image_t img;
    bgil::read_image(filename, img, bgil::gdal_tag());

    return 0;
}