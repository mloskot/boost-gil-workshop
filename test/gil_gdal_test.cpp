#include <exception>
#include <iostream>
#include <string>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/gdal_all.hpp>
namespace bgil = boost::gil;

int main()
{
    try
    {
        std::string fname("G:\\dev\\boost\\_git\\boost-gil-extension\\images\\utm.tif");
        bgil::image_read_info<bgil::gdal_tag> info = bgil::read_image_info(fname, bgil::gdal_tag());


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
