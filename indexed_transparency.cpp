#include <iostream>
#include <string>
#include <vector>
#include <boost/gil.hpp>
#include <boost/gil/extension/toolbox/toolbox.hpp>
#include <boost/gil/extension/io/png.hpp>
namespace gil = boost::gil;
using namespace std;

int main()
{
    std::vector<gil::rgb8_pixel_t> palette{
        {255,   0,   0},
        {  0, 255,   0},
        {  0,   0, 255},
        {255, 255, 255}
    };
    auto palette_view = gil::interleaved_view(palette.size(), 1,
        reinterpret_cast<gil::rgb8_pixel_t const*>(&palette[0]),
        palette.size() * sizeof(gil::rgb8_pixel_t));

    int w = 2, h =2;
    std::vector<std::uint8_t> indices{0, 1, 2, 3};
    auto indices_view = gil::interleaved_view(w, h,
        reinterpret_cast<gil::gray8_pixel_t const*>(&indices[0]), w);

    auto src_view = view(indices_view, palette_view);
    for (auto const& p : src_view)
    {
        gil::rgb8_pixel_t const & c = p;
        std::cout << (int)c[0] << '\t' << (int)c[1] << '\t' << (int)c[2] << std::endl;
    }

    {
        gil::rgba8_image_t dst_img(w, h);
        gil::copy_and_convert_pixels(src_view, gil::view(dst_img));
        gil::write_view("src.png", gil::view(dst_img), gil::png_tag());
    }

    {
        struct color_convert_from_indexed_transparency
        {
            gil::rgb8_pixel_t transparent_;
            color_convert_from_indexed_transparency(gil::rgb8_pixel_t transparent) : transparent_(transparent) {}

            void operator()(gil::rgb8_pixel_t const& src, gil::rgba8_pixel_t& dst) const
            {
                if (src == transparent_)
                {
                    get_color(dst, gil::red_t()) = get_color(src, gil::red_t());
                    get_color(dst, gil::green_t()) = get_color(src, gil::green_t());
                    get_color(dst, gil::blue_t()) = get_color(src, gil::blue_t());
                    get_color(dst, gil::alpha_t()) = 0;
                }
                else
                {
                    gil::default_color_converter()(src, dst);
                }
            }
        };

        gil::rgba8_image_t dst_img(w, h);

        auto src_rgba_view = gil::color_converted_view<gil::rgba8_pixel_t>(src_view, color_convert_from_indexed_transparency(palette[3]));
        gil::copy_and_convert_pixels(src_rgba_view, gil::view(dst_img));
        gil::write_view("dst1.png", gil::view(dst_img), gil::png_tag());
    }



    return 0;
}


#if 0
    auto const image_file = "d:\\data\\PngSuite-2017jul19\\tbbn3p08.png";

    gil::image_read_settings<gil::png_tag> settings;
    settings._read_file_gamma        = true;
    settings._read_transparency_data = true;
    settings._read_palette = true;

    auto backend = gil::read_image_info(image_file, settings);

    gil::rgba8_image_t img;
    gil::read_image(image_file, img, settings);
#endif

