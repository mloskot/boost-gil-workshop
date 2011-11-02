#include <exception>
#include <iostream>
#include <string>
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
            fname = "/home/mloskot/data/gdal/utm.tif";
        }
  
        bgil::image_read_info<bgil::gdal_tag> info = bgil::read_image_info(fname, bgil::gdal_tag());

        std::cout << info.width << " x " << info.height << std::endl;

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
