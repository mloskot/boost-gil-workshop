#include <array>
#include <cassert>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <boost/algorithm/hex.hpp>
#include <boost/gil/gil_all.hpp>
//#include <boost/gil/extension/toolbox/image_types/indexed_image.hpp>
#include <boost/gil/extension/toolbox/toolbox.hpp>
#include <boost/gil/extension/io/tiff_write.hpp>
#include <boost/gil/extension/io/png_write.hpp>
namespace fs = std::experimental::filesystem;
namespace gi = boost::gil;

struct benchmark
{
    using time_point = decltype(std::chrono::high_resolution_clock::now());
    time_point start{std::chrono::high_resolution_clock::now()};
    std::string name;
    benchmark(std::string name) : name(name) {}
    ~benchmark()
    {
        auto const ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
        std::cout << "[benchmark] " << name << ":\t" << ms << " ms " << std::endl;
    }
};

using dims_t = gi::point2<std::ptrdiff_t>;
using tile_t = std::tuple<std::string, int, int, int, int>;
using tiles_t = std::vector<tile_t>;

template <typename View>
void dump_view(std::ostream& os, View const& view)
{
    using pixel_t = gi::pixel<typename gi::channel_type<View>::type, gi::layout<typename gi::color_space_type<View>::type>>;
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

template <typename PixelType, typename Iterator>
auto make_interleaved_view(std::size_t width, std::size_t height, Iterator pixels, std::ptrdiff_t rowsize_in_bytes)
{
    auto it = reinterpret_cast<PixelType*>(pixels);
    return gi::interleaved_view(width, height, it, rowsize_in_bytes);
}

auto read_tile_dib(tile_t const& tile)
{
    std::string hex, bin;
    {
        benchmark bench{"read"};
        hex.reserve(std::get<3>(tile) * std::get<4>(tile) * 2);
        assert(hex.size() % 2 == 0);
        bin.reserve(hex.size() / 2);
        std::ifstream ifs{std::get<0>(tile)};
        ifs >> hex;
        assert(hex.size() % 2 == 0);
    }
    {
        benchmark bench{"unhex"};
        bin = boost::algorithm::unhex(hex);
        assert(bin.size() * 2 == hex.size());
    }
    return std::vector<unsigned char>{bin.cbegin(), bin.cend()};
}

auto read_tile_clr(tile_t const& tile)
{
    auto f = fs::path(std::get<0>(tile));
    f.replace_extension(".clr");

    std::tuple<int, std::array<gi::rgb8_pixel_t, 256>> palette;
    if (fs::exists(f))
    {
        std::ifstream ifs{f};
        std::string line;
        std::getline(ifs, line);
        std::get<0>(palette)= std::stoi(line);
        auto& rgb = std::get<1>(palette);
        std::size_t i{0};
        while (std::getline(ifs, line))
        {
            unsigned char r{0}, g{0}, b{0};
            std::sscanf(line.c_str(), "%hhu,%hhu,%hhu", &r, &g, &b);
            rgb[i++] = {r, g, b};
        }
    }
    return palette;
}

int main()
{
    try
    {
        auto const this_path = fs::canonical(fs::path(__FILE__).parent_path());
        auto const tiles2x2_path = this_path / fs::path("images") / "osm" / "2x2";
        auto const tiles4x4_path = this_path / fs::path("images") / "osm" / "4x4";
        auto const tiles_path = tiles4x4_path;

        tiles_t tiles;
        {
            benchmark bench{"collect tiles"};
            for (auto& f : fs::directory_iterator(tiles_path.string()))
            {
                if (f.path().extension() != ".hex")
                    continue;

                int x, y, w, h;
                std::sscanf(f.path().filename().string().c_str(), "%dx%d_%dx%d", &x, &y, &w, &h);
                printf("%s tile at %dx%d size %dx%d\n", f.path().string().c_str(), x, y, w, h);
                tiles.emplace_back(f.path().string(), x, y, w, h);
            }
            assert(tiles.size() > 0 && tiles.size() % 2 == 0);
        }

        auto const matrix_size = static_cast<std::size_t>(std::log2(tiles.size()));
        dims_t const tile_dims{std::get<3>(*tiles.begin()), std::get<4>(*tiles.begin())};
        dims_t const mosaic_dims{static_cast<ptrdiff_t>(tile_dims.x * matrix_size), static_cast<ptrdiff_t>(tile_dims.x * matrix_size)};

        gi::rgb8_image_t mosaic_img{mosaic_dims};
        auto mosaic_view = gi::view(mosaic_img);
        //{
        //    benchmark bench{"fill mosaic"};
        //    gi::rgb8_pixel_t white8(255, 255, 255);
        //    gi::fill_pixels(mosaic_view, white8);
        //}
        {
            auto const out = tiles_path / "mosaic-blank.png";
            benchmark bench{"dump " + out.string()};
            gi::write_view(out.string(), mosaic_view, gi::png_tag());
        }
        {
            benchmark bench{"merge mosaic total"};
            for (auto &t : tiles)
            {
                int x, y, w, h;
                std::tie(std::ignore, x, y, w, h) = t;

                int num_colors;
                std::array<gi::rgb8_pixel_t, 256> colors;
                std::tie(num_colors, colors) = read_tile_clr(t);
                auto palette_view = gi::interleaved_view(colors.size(), 1, (gi::rgb8_image_t::view_t::x_iterator) colors.data(), colors.size() * sizeof(gi::rgb8_pixel_t));

                auto dib = read_tile_dib(t);
                auto indices_view = gi::interleaved_view(w, h, (gi::gray8_image_t::view_t::x_iterator) dib.data(), w);
                auto ii_view = view(indices_view, palette_view);

                gi::rgb8_image_t img(ii_view.dimensions());
                auto tile_view = view(img);
                gi::copy_pixels(ii_view, view(img));
                //{
                //    auto const out = std::get<0>(t) + ".dump";
                //    benchmark bench1{"dump " + out};
                //    std::ofstream ofs(out);
                //    dump_view(ofs, tile_view);
                //}
                {
                    benchmark bench1{"copy " + std::get<0>(t)};
                    gi::copy_and_convert_pixels(
                        tile_view,
                        gi::subimage_view(mosaic_view, x * w, y * h, w, h));
                }
            }
        }
        {
            auto const out = tiles_path / "mosaic-tiles";
            benchmark bench{"dump " + out.string()};
            gi::write_view(out.string() + ".png", mosaic_view, gi::png_tag());
            //gi::write_view(out.string() + ".tif", mosaic_view, gi::tiff_tag());
        }
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
