#include <algorithm>
#include <iostream>
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/pnm_all.hpp>
//#include <boost/fusion/sequence.hpp>
//#include <boost/fusion/adapted/struct/adapt_struct.hpp>
//#include <boost/fusion/include/adapt_struct.hpp>
using std::cout;
using std::endl;
namespace bgil = boost::gil;

int main()
{
    std::string fname("D:\\dev\\gil\\_git\\boost-gil-workshop\\images\\sample.pgm");
    auto info = bgil::read_image_info(fname, bgil::pnm_tag());
 
    bgil::gray8_image_t img;
    bgil::read_image(fname, img, bgil::pnm_tag()); 

    unsigned cnt(0);
    bgil::gray8_image_t::view_t vimg(bgil::view(img));
 
    return 0;
}