#include <exception>
#include <iostream>
#include <string>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/gdal_all.hpp>
namespace bgil = boost::gil;

int main(int argc, char** argv)
{
    try
    {
        std::string fname;
        if (argc > 1)
            fname = argv[1];
        else
        {
            // XXX: Change if needed
            fname = "D:\\dev\\gdal\\_svn\\trunk\\autotest\\gdrivers\\data\\utm.tif";
        }
  
        typedef bgil::gdal_tag fmt_tag;
        bgil::image_read_info<fmt_tag> info = bgil::read_image_info(fname, fmt_tag());
        std::cout << info._width << " x " << info._height << std::endl;

        bgil::gray8_image_t img;
        bgil::read_image(fname, img, fmt_tag()); 
    
        unsigned cnt(0);
        bgil::gray8_image_t::view_t vimg(bgil::view(img));
    
    }
    catch (std::ios_base::failure const& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
