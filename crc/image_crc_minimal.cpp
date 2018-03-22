#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#ifdef _MSC_VER
#pragma warning(disable:4244)
#pragma warning(disable:4503)
#pragma warning(disable:4714)
#pragma warning(disable:4996)
#endif
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/mpl/vector.hpp>
using namespace std;
using namespace boost::gil;
namespace fs = std::experimental::filesystem;

#ifdef NDEBUG
auto const config_suffix = "opt";
#else
auto const config_suffix = "dbg";
#endif
#if defined(_WIN64) || defined(__x86_64__) || defined(__LLP64__) || (UINTPTR_MAX == 0xffffffffffffffffULL)
auto const arch_suffix = "x64";
#else
auto const arch_suffix = "x32";
#endif

struct image_test
{
    std::ofstream log_;
    image_test(std::string const& path) : log_(path) { cout << path << endl; }

    void print_checksum(const rgb8c_view_t& img_view, const string& name)
    {
        boost::crc_32_type checksum_acumulator;
        checksum_acumulator.process_bytes(img_view.row_begin(0), img_view.size() * 3);
        cout << "Checksum (" << name << "):\t" << std::hex << checksum_acumulator.checksum() << "   <- rgb8" << endl;
        log_ << "Checksum (" << name << "):\t" << std::hex << checksum_acumulator.checksum() << "   <- rgb8" << endl;
    }

    void print_view_hex(const rgb8c_view_t& img_view)
    {
        auto const raw_size = img_view.size() * 3;
        unsigned char const* raw_ptr = &img_view[0][0];
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

    template <typename Image>
    void bit_aligned_pixel_image_test(typename Image::point_t const& dims, std::string const& bgr_suffix)
    {
        std::cout << "----------" << bgr_suffix << "----------" << endl;

        // make empty image
        Image img(dims);
        auto const& img_view = view(img);
        //check_view(img_view, "make"); // possibly random garbage

        // colors
        rgb8_pixel_t red8(255, 0, 0), green8(0, 255, 0), blue8(0, 0, 255), white8(255, 255, 255);
        typename Image::view_t::value_type red, green, blue, white;
        color_convert(red8, red);
        color_convert(green8, green);
        color_convert(blue8, blue);
        color_convert(white8, white);

        // fill it with red
        fill(img_view.begin(), img_view.end(), red);
        check_view(img_view, bgr_suffix +": red  fill");

        // draw a blue line along the diagonal
        {
            auto loc = img_view.xy_at(0, img_view.height() - 1);
            for (int y = 0; y < img_view.height(); ++y)
            {
                *loc = blue;
                ++loc.x();
                --loc.y();
            }
            check_view(img_view, bgr_suffix +": blue line");
        }
    }
};

// XXX: for some images eg. 5x5, may often randomly crash in opt, in gil::image<..>::deallocate
int main()
{
    try
    {
        auto const this_path = fs::canonical(fs::path(__FILE__).parent_path());
        auto const log_base_path = this_path / fs::path("test_");

        using bgr111_img_t = image<
            bit_aligned_pixel_reference<boost::uint8_t, boost::mpl::vector3_c<int, 1, 1, 1>, bgr_layout_t, true> const, false>;
        using bgr121_img_t = image<
            bit_aligned_pixel_reference<boost::uint8_t, boost::mpl::vector3_c<int, 1, 2, 1>, bgr_layout_t, true> const, false>;
        using bgr222_img_t = image<
            bit_aligned_pixel_reference<boost::uint8_t, boost::mpl::vector3_c<int, 2, 2, 2>, bgr_layout_t, true> const, false>;
        using bgr232_img_t = image<
            bit_aligned_pixel_reference<boost::uint8_t, boost::mpl::vector3_c<int, 2, 3, 2>, bgr_layout_t, true> const, false>;
        using bgr233_img_t = image<
            bit_aligned_pixel_reference<boost::uint8_t, boost::mpl::vector3_c<int, 2, 3, 3>, bgr_layout_t, true> const, false>;

        auto const max_dim = 6; // modify if you need
        for (int i = 2; i < max_dim + 1; i += 1)
        {
            std::ostringstream os;
            os << log_base_path << i << "x" << i << "_" << arch_suffix << "_" << config_suffix << ".log";
            image_test test(os.str());

            test.bit_aligned_pixel_image_test<bgr111_img_t>({i, i}, "bgr111");
            test.bit_aligned_pixel_image_test<bgr121_img_t>({i, i}, "bgr121");
            test.bit_aligned_pixel_image_test<bgr222_img_t>({i, i}, "bgr222");
            test.bit_aligned_pixel_image_test<bgr232_img_t>({i, i}, "bgr232");
            test.bit_aligned_pixel_image_test<bgr233_img_t>({i, i}, "bgr233");
        }
    }
    catch (std::runtime_error const& e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::string out("type & ENTER ");
    std::cout << out;
    std::cin >> out;
    return 0;
}
