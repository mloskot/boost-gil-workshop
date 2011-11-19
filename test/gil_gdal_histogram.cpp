// Copyright 2005-2007 Adobe Systems Incorporated
//    
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
// 
// See http://opensource.adobe.com/gil for most recent version including documentation.

/// \brief Example file to demonstrate a way to compute histogram
/// \author Lubomir Bourdev and Hailin Jin
/// \date February 27, 2007
/// \modified Mateusz Loskot <mateusz@loskot.net> 
/// \date November, 2011

#include <algorithm>
#include <fstream>
#include <string>
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/color_convert.hpp>
#include <boost/gil/extension/io_new/gdal_all.hpp>

using namespace boost::gil;

template <typename GrayView, typename R>
void gray_image_hist(const GrayView& img_view, R& hist)
{
    //for_each_pixel(img_view,++lambda::var(hist)[lambda::_1]);
    for (typename GrayView::iterator it=img_view.begin(); it!=img_view.end(); ++it)
        ++hist[*it];
}

template <typename V, typename R>
void get_hist(const V& img_view, R& hist)
{
    gray_image_hist(color_converted_view<gray8_pixel_t>(img_view), hist);
}

int main()
{
    //std::string fname = "D:\\dev\\gdal\\_svn\\trunk\\autotest\\gdrivers\\data\\utm.tif";
    std::string fname = "D:\\dev\\gdal\\_svn\\trunk\\autotest\\gcore\\data\\sasha.tif";

    gray8_image_t img;
    read_image(fname, img, gdal_tag()); 

    int histogram[256] = { 0 };
    //std::fill(histogram,histogram+256,0);
    get_hist(const_view(img), histogram);

    std::fstream histo_file("out-histogram.txt",std::ios::out);
    for(std::size_t ii=0;ii<256;++ii)
        histo_file << histogram[ii] << std::endl;
    histo_file.close();

    return 0;
}
