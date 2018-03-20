#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#pragma warning(disable:4244)
#pragma warning(disable:4503)
#pragma warning(disable:4996)
#include <string>
#include <ios>
#include <iostream>
#include <fstream>
#include <boost/crc.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
using namespace boost::gil;
using namespace std;
using namespace boost;

extern boost::gil::rgb8c_planar_view_t sample_view;

void error_if(bool condition)
{
    if (condition)
        throw std::exception();
}

struct image_test
{
    void run();
    void print_checksum(const rgb8c_view_t& img_view, const string& name)
    {
        boost::crc_32_type checksum_acumulator;
        checksum_acumulator.process_bytes(img_view.row_begin(0), img_view.size() * 3);
        cerr << "Checksum (" << name << "):\t\t" << std::hex << checksum_acumulator.checksum() << endl;
    }

    template <typename View>
    void check_view(const View& img_view, const string& name)
    {
        rgb8_image_t rgb_img(img_view.dimensions());
        copy_and_convert_pixels(img_view, view(rgb_img));
        print_checksum(const_view(rgb_img), name);
    }

    template <typename Img>
    void basic_test(const string& prefix);
    template <typename Img>
    void image_all_test(const string& prefix);
    template <typename View>
    void view_transformations_test(const View& img_view, const string& prefix);
};

// testing image iterators, clone, fill, locators, color convert
template <typename Img>
void image_test::basic_test(const string& prefix)
{
    typedef typename Img::view_t View;

    // make a 20x20 image
    Img img(typename View::point_t(20, 20));
    const View& img_view = view(img);
    check_view(img_view, "make");

    // fill it with red
    rgb8_pixel_t red8(255, 0, 0), green8(0, 255, 0), blue8(0, 0, 255), white8(255, 255, 255);
    typename View::value_type red, green, blue, white;
    color_convert(red8, red);
    default_color_converter()(red8, red);
    red = color_convert_deref_fn<rgb8_ref_t, typename Img::view_t::value_type>()(red8);

    color_convert(green8, green);
    color_convert(blue8, blue);
    color_convert(white8, white);
    fill(img_view.begin(), img_view.end(), red);
    check_view(img_view, "fill");

    color_convert(red8, img_view[0]);
    check_view(img_view, "convert");

    // pointer to first pixel of second row
    //typename View::reference rt = img_view.at(0, 0)[img_view.width()];
    //typename View::x_iterator ptr = &rt;
    //typename View::reference rt2 = *(img_view.at(0, 0) + img_view.width());
    //typename View::x_iterator ptr2 = &rt2;
    //error_if(ptr != ptr2);
    //error_if(img_view.x_at(0, 0) + 10 != 10 + img_view.x_at(0, 0));

    // draw a blue line along the diagonal
    typename View::xy_locator loc = img_view.xy_at(0, img_view.height() - 1);
    //for (int y = 0; y < img_view.height(); ++y)
    //{
    //    *loc = blue;
    //    ++loc.x();
    //    loc.y()--;
    //}
    //check_view(img_view, "blue line");

    // draw a green dotted line along the main diagonal with step of 3
    loc = img_view.xy_at(img_view.width() - 1, img_view.height() - 1);
    while (loc.x() >= img_view.x_at(0, 0))
    {
        *loc = green;
        loc -= typename View::point_t(3, 3);
    }
    check_view(img_view, "green line");

    // Clone and make every red pixel white
    Img imgWhite(img);
    for (typename View::iterator it = view(imgWhite).end(); (it - 1) != view(imgWhite).begin(); --it)
    {
        if (*(it - 1) == red)
            *(it - 1) = white;
    }
    check_view(img_view, "final red");
    check_view(view(imgWhite), "final white");
}

template <typename Img>
void image_test::image_all_test(const string& prefix)
{
    check_view(sample_view, "sample_view");

    basic_test<Img>(prefix);

    Img img;
    img.recreate(sample_view.dimensions());
    copy_and_convert_pixels(sample_view, view(img));
    check_view(view(img), "copy");
    check_view(rotated90cw_view(view(img)), "90cw");
    check_view(flipped_up_down_view(view(img)), "flipped_ud");
}

template <typename View>
void image_test::view_transformations_test(const View& img_view, const string& prefix)
{

}

void image_test::run()
{
    using bgr121_ref_t = const bit_aligned_pixel_reference<boost::uint8_t, mpl::vector3_c<int, 1, 2, 1>, bgr_layout_t, true>;
    using bgr121_image_t = image<bgr121_ref_t, false>;
    image_all_test<bgr121_image_t>("bgr121_");
}

int main(int argc, char* argv[])
{
    try
    {
        image_test test;
        test.run();

        std::cin >> std::string{};
        return EXIT_SUCCESS;
    }
    catch (std::runtime_error const& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unknown error" << std::endl;
    }
    std::cin >> std::string{};
    return EXIT_FAILURE;
}
