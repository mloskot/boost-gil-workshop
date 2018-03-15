#include <exception>
#include <iostream>
#include <boost/gil/extension/io/pnm_all.hpp>

namespace bg = boost::gil;

int main()
{
    try
    {
        std::string fname("D:\\dev\\gil\\_git\\boost-gil-workshop\\images\\sample.pgm");
        auto info = bg::read_image_info(fname, bg::pnm_tag());

        bg::gray8_image_t img;
        bg::read_image(fname, img, bg::pnm_tag());
        bg::gray8_image_t::view_t vimg(bg::view(img));

        return EXIT_SUCCESS;
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return EXIT_FAILURE;
}
