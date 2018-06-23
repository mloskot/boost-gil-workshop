#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#pragma warning(disable:4244)
#pragma warning(disable:4503)
#pragma warning(disable:4996)
#include <string>
#include <ios>
#include <iostream>
#define HAS_NO_FILESYSTEM
#ifndef HAS_NO_FILESYSTEM
#include <filesystem>
#endif
#include <fstream>
#include <string>
#include <vector>
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
using namespace std;
using namespace boost;
using namespace boost::gil;
#ifndef HAS_NO_FILESYSTEM
namespace fs = std::experimental::filesystem;
#endif

//#define ENABLE_SAMPLE_VIEW_TESTS
#ifdef ENABLE_SAMPLE_VIEW_TESTS
extern boost::gil::rgb8c_planar_view_t sample_view;
#endif

struct image_test
{
    std::ostream& log_;
    image_test(std::ostream& os) : log_(os) {}
    void run();

    void print_checksum(const rgb8c_view_t& img_view, const string& name)
    {
        boost::crc_32_type checksum_acumulator;
        checksum_acumulator.process_bytes(img_view.row_begin(0), img_view.size() * 3);
        cerr << "Checksum (" << name << "):\t\t" << std::hex << checksum_acumulator.checksum() << endl;
        log_ << "Checksum (" << name << "):\t\t" << std::hex << checksum_acumulator.checksum() << endl;
    }

    void print_view_hex(const rgb8c_view_t& img_view)
    {
        auto const raw_size = img_view.size() * 3;
        unsigned char const* raw_ptr=&img_view[0][0];

        std::vector<unsigned char> raw(raw_ptr, raw_ptr + raw_size);
        std::string out(raw_size * 2, char{0});
        boost::algorithm::hex(raw, out.begin());
        log_ << out << endl;
    }

    template <typename View>
    void check_view(const View& img_view, const string& name)
    {
        rgb8_image_t rgb_img(img_view.dimensions());
        copy_and_convert_pixels(img_view, view(rgb_img));
        print_checksum(const_view(rgb_img), name);
        print_view_hex(const_view(rgb_img));
    }

    template <typename Img>
    void basic_test(const string& prefix);
    template <typename Img>
    void image_all_test(const string& prefix);
};

// testing image iterators, clone, fill, locators, color convert
template <typename Img>
void image_test::basic_test(const string& prefix)
{
    typedef typename Img::view_t View;

    // make a 20x20 image
    Img img(typename View::point_t(20, 20));
    const View& img_view = view(img);
    //check_view(img_view, "make"); // possibly random garbage

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

    //color_convert(red8, img_view[0]);
    //check_view(img_view, "convert");

    // draw a blue line along the diagonal
    typename View::xy_locator loc = img_view.xy_at(0, img_view.height() - 1);
    for (int y = 0; y < img_view.height(); ++y)
    {
        *loc = blue;
        ++loc.x();
        loc.y()--;
    }
    check_view(img_view, "blue line");

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
#ifdef ENABLE_SAMPLE_VIEW_TESTS
    check_view(sample_view, "sample_view");
#endif

    basic_test<Img>(prefix);

#ifdef ENABLE_SAMPLE_VIEW_TESTS
    Img img;
    img.recreate(sample_view.dimensions());
    copy_and_convert_pixels(sample_view, view(img));
    check_view(view(img), "copy");
    check_view(rotated90cw_view(view(img)), "90cw");
    check_view(flipped_up_down_view(view(img)), "flipped_ud");
#endif
}

void image_test::run()
{
    using bgr121_ref_t = const bit_aligned_pixel_reference<boost::uint8_t, mpl::vector3_c<int, 1, 2, 1>, bgr_layout_t, true>;
    using bgr121_image_t = image<bgr121_ref_t, false>;
    image_all_test<bgr121_image_t>("bgr121_");
}

#ifdef _MSC_VER
auto pause = []() { std::string s; std::cin >> s; };
#define PAUSE_ME pause()
#else
#define PAUSE_ME (void)0
#endif

int main(int argc, char* argv[])
{
    try
    {
        std::ostream* pos;
#ifndef HAS_NO_FILESYSTEM
        auto const this_path = fs::canonical(fs::path(__FILE__).parent_path());
        auto const log_path = this_path / fs::path("last.log");
        std::ofstream ofs(log_path);
        pos = &ofs;
#else
        pos = &std::cout;
#endif
        {
            image_test test(*pos);
            test.run();
        }

        PAUSE_ME;
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

    PAUSE_ME;
    return EXIT_FAILURE;
}
