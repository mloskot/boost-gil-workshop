#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <boost/algorithm/hex.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/toolbox/image_types/indexed_image.hpp>
using namespace std;
using namespace boost::gil;

template <typename View>
void dump_view(std::ostream& os, View const& view)
{
    using pixel_t = pixel<typename channel_type<View>::type, layout<typename color_space_type<View>::type>>;
    std::vector<pixel_t> row_buf(view.width());
    std::string row_hex(2 * row_buf.size() * sizeof(pixel_t), char{0});
    for (std::ptrdiff_t y = 0; y < view.height(); ++y)
    {
        std::copy(view.row_begin(y), view.row_end(y), row_buf.begin());

        auto beg = reinterpret_cast<unsigned char*>(row_buf.data());
        auto end = beg + row_buf.size() * sizeof(pixel_t);

        boost::algorithm::hex(beg, end, row_hex.begin());
        os << row_hex << '\n';
    }
}

int main()
{
    try
    {
        rgb8_pixel_t red8(255, 0, 0), green8(0, 255, 0), blue8(0, 0, 255), white8(255, 255, 255);

        int w = 4, h =4;
        vector<uint8_t> indices(w * h, 0); // fill white
        vector<rgb8_pixel_t> palette{white8, blue8};

        auto indices_view = interleaved_view(w, h, (gray8_image_t::view_t::x_iterator) indices.data(), w);
        auto loc = indices_view.xy_at(0, indices_view.height() - 1);
        for (int y = 0; y < indices_view.height(); ++y)
        {
            *loc = 1;
            ++loc.x();
            loc.y()--;
        }

        auto palette_view = interleaved_view(2, 1, (rgb8_image_t::view_t::x_iterator) palette.data(), 2 * 3);
        auto ii_view = view(indices_view, palette_view);

        rgb8_image_t img(ii_view.dimensions());
        auto img_view = view(img);
        fill_pixels(img_view, white8);

        copy_pixels(ii_view, view(img));
        dump_view(std::cout, view(img));

        return EXIT_SUCCESS;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unknown error\n";
    }

    std::string m{"type <ENTER> to exit\n"};
    std::cout << m;
    std::cin >> m;
    return 0;
}
