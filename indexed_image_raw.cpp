#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/toolbox/image_types/indexed_image.hpp>

using namespace std;
using namespace boost::gil;

int main()
{
    try
    {
        vector<uint8_t> indices(2 * 2);
        vector<rgb8_pixel_t> palette(2);

        auto indices_view = interleaved_view(2, 2
            , (gray8_image_t::view_t::x_iterator) indices.data()
            , 2 // row size in bytes
        );
        auto palette_view = interleaved_view(2, 1
            , (rgb8_image_t::view_t::x_iterator) palette.data()
            , 2 * 3 // row size in bytes
        );

        auto i = indices_view.begin();
        *i = 0; i++;
        *i = 1; i++;
        *i = 0; i++;
        *i = 1; i++;
        assert(i == indices_view.end());

        auto c = palette_view.begin();
        *c =  rgb8_pixel_t(255, 0, 0); c++;
        *c =  rgb8_pixel_t(0, 0, 255); c++;
        assert(c == palette_view.end());

        using image_t = indexed_image_deref_fn<gray8_image_t::const_view_t::xy_locator, rgb8_image_t::const_view_t::xy_locator>;
        image_t ii(indices_view.xy_at(0, 0), palette_view.xy_at(0, 0));
        auto p00 = ii(point_t(0, 0));
        assert(p00[0] == 255 && p00[1] == 0 && p00[2] == 0);
        auto p10 = ii(point_t(1, 0));
        assert(p10[0] == 0 && p10[1] == 0 && p10[2] == 255);
        auto p01 = ii(point_t(0, 1));
        assert(p01[0] == 255 && p01[1] == 0 && p01[2] == 0);
        auto p11 = ii(point_t(1, 1));
        assert(p11[0] == 0 && p11[1] == 0 && p11[2] == 255);
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
