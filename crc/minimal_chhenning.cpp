// https://github.com/boostorg/gil/issues/49#issuecomment-377725324
#include <boost/gil/gil_all.hpp>
//#define SAVE_BMP
#ifdef SAVE_BMP
#include <boost/gil/extension/io/bmp_all.hpp>
#endif
using namespace boost::gil;

int main()
{
    using ref_t = const bit_aligned_pixel_reference<boost::uint8_t, boost::mpl::vector3_c<int, 1, 2, 1>, bgr_layout_t, true>;
    using ptr_t = bit_aligned_pixel_iterator<ref_t>;
    using pixel_t = std::iterator_traits<ptr_t>::value_type;
    using bgr121_image_t = image<ref_t, false>;

    // works
    {
        bgr121_image_t a(10, 10);
        fill_pixels(view(a), pixel_t(0, 0, 1));

        auto loc = view(a).xy_at(0, 0);
        for (int y = 0; y < view(a).height(); ++y)
        {
            *loc = pixel_t(1, 0, 0);
            ++loc.x();
            ++loc.y();
        }
#ifdef SAVE_BMP
        auto v = color_converted_view<rgb8_pixel_t>(view(a));
        write_view("a.bmp", v, bmp_tag());
#endif
    }
    // doesn't work
    {
        bgr121_image_t b(10, 10);
        fill_pixels(view(b), pixel_t(0, 0, 1));
        auto loc = view(b).xy_at(0, view(b).height() - 1);
        for (int y = 0; y < view(b).height(); ++y)
        {
            *loc = pixel_t(1, 0, 0);
            ++loc.x();
            --loc.y();
        }
#ifdef SAVE_BMP
        auto v = color_converted_view<rgb8_pixel_t>(view(b));
        write_view("b.bmp", v, bmp_tag());
#endif
    }
}
