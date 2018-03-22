#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#pragma warning(disable:4244)
#pragma warning(disable:4503)
#pragma warning(disable:4714)
#pragma warning(disable:4996)
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
using namespace std;
using namespace boost::gil;
namespace fs = std::experimental::filesystem;

struct image_test
{
    std::ostream& log_;
    image_test(std::ostream& os) : log_(os) {}

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

    template <typename Img>
    void basic_test(typename Img::point_t const& dims)
    {
        using view_value_t = typename Img::view_t::value_type;

        // make empty image
        Img img(dims);
        auto const& img_view = view(img);
        //check_view(img_view, "make"); // possibly random garbage

        // colors
        rgb8_pixel_t red8(255, 0, 0), green8(0, 255, 0), blue8(0, 0, 255), white8(255, 255, 255);
        view_value_t red, green, blue, white;
        color_convert(red8, red);
        color_convert(green8, green);
        color_convert(blue8, blue);
        color_convert(white8, white);

        // fill it with red
        fill(img_view.begin(), img_view.end(), red);
        check_view(img_view, "red  fill");

        // draw a blue line along the diagonal
        {
            auto loc = img_view.xy_at(0, img_view.height() - 1);
            for (int y = 0; y < img_view.height(); ++y)
            {
                *loc = blue;
                ++loc.x();
                loc.y()--;
            }
            check_view(img_view, "blue line");
        }
    }
};

#ifdef NDEBUG
auto const config_suffix = "opt";
#else
auto const config_suffix = "dbg";
#endif
#ifdef _WIN64
auto const arch_suffix = "x64";
#else
auto const arch_suffix = "x32";
#endif

int main()
{
    try
    {
        using bgr121_ref_t = const bit_aligned_pixel_reference<boost::uint8_t, boost::mpl::vector3_c<int, 1, 2, 1>, bgr_layout_t, true>;
        using bgr121_image_t = image<bgr121_ref_t, false>;
        using dims_t = bgr121_image_t::point_t;

        auto const this_path = fs::canonical(fs::path(__FILE__).parent_path());

        auto const max_dim = 8; // modify if you need
        for (int i = 2; i < max_dim + 1; i += 1)
        {
            auto const log_path = this_path / fs::path("test_");
            std::ostringstream os;
            os << log_path << i << "x" << i << "_" << arch_suffix << "_" << config_suffix << ".log";
            auto log = os.str();

            // if (i == 5) may randomly crash in opt, in gil::image<..>::deallocate

            // test
            std::ofstream ofs(log );
            std::cerr << log << std::endl;
            image_test test(ofs);
            dims_t dims(i, i);
            test.basic_test<bgr121_image_t>(dims);
        }
    }
    catch (std::runtime_error const& e)
    {
        std::cerr << e.what() << std::endl;
    }

    //std::string out("type & ENTER ");
    //std::cerr << out;
    //std::cin >> out;
    return 0;
}
