#include <algorithm>
#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/pnm_all.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
using std::cout;
using std::endl;
namespace bgil = boost::gil;

int main()
{
    std::string fname("G:\\dev\\boost\\_git\\boost-gil-extension\\images\\sample.pgm");
    bgil::image_read_info<bgil::pnm_tag> info = bgil::read_image_info(fname, bgil::pnm_tag());
 
    bgil::gray8_image_t img;
    bgil::read_image(fname, img, bgil::pnm_tag()); 

    unsigned cnt(0);
    auto vimg = bgil::view(img);
    for (auto it = vimg.begin(); it != vimg.end(); ++it)
    {
        ++cnt;
    }
    cout << cnt << endl;
 
    return 0;
}