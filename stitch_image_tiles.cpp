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
#include <boost/gil/extension/io/png_write.hpp>
namespace fs = std::experimental::filesystem;
namespace gil = boost::gil;

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

using dims_t = gil::point2<std::ptrdiff_t>;
using tile_t = std::tuple<std::string, int, int, int, int>;
using tiles_t = std::vector<tile_t>;

template <typename PixelType, typename Iterator>
auto make_interleaved_view(std::size_t width, std::size_t height, Iterator pixels, std::ptrdiff_t rowsize_in_bytes)
{
    auto it = reinterpret_cast<PixelType*>(pixels);
    return ::boost::gil::interleaved_view(width, height, it, rowsize_in_bytes);
}

auto read_tile_dib(tile_t const& tile)
{
    std::string hex, bin;
    {
        benchmark bench{"read"};
        hex.reserve(std::get<3>(tile) * std::get<4>(tile) * 2);
        assert(hex.size() % 2 == 0);
        bin.reserve(hex.size() / 2);
        std::ifstream ifs{ std::get<0>(tile) };
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

int main()
{
    try
    {
        //std::string const tiles_dir = "f:\\tmp\\tiles\\osm-2x2";
        std::string const tiles_dir = "f:\\tmp\\tiles\\osm-4x4";

        tiles_t tiles;
        {
            benchmark bench{"find tiles"};
            for (auto& f : fs::directory_iterator(tiles_dir))
            {
                auto n = f.path().filename().string();
                if (n.substr(n.size() - 3) != "dib")
                    continue;

                int x, y, w, h;
                std::sscanf(n.c_str(), "%dx%d_%dx%d", &x ,&y, &w, &h);
                printf("%s tile at %dx%d size %dx%d\n", f.path().string().c_str(), x, y, w, h);
                tiles.emplace_back(f.path().string(), x, y, w, h);
            }
            assert(tiles.size() % 2 == 0);
        }

        auto const matrix_size = static_cast<std::size_t>(std::log2(tiles.size()));
        dims_t const tile_dims{std::get<3>(*tiles.begin()), std::get<4>(*tiles.begin())};
        dims_t const mosaic_dims{static_cast<ptrdiff_t>(tile_dims.x * matrix_size), static_cast<ptrdiff_t>(tile_dims.x * matrix_size)};

        gil::gray8_image_t mosaic_img{mosaic_dims};
        auto mosaic_view = gil::view(mosaic_img);
        {
            benchmark bench{"fill mosaic"};
            gil::fill_pixels(mosaic_view, 255);
        }
        {
            benchmark bench{"write blank"};
            gil::write_view(tiles_dir + "\\mosaic-blank.png", mosaic_view, gil::png_tag());
        }
        {
            benchmark bench{"merge mosaic total"};
            for (auto &t : tiles)
            {
                int x, y, w, h;
                std::tie(std::ignore, x, y, w, h) = t;

                auto dib = read_tile_dib(t);
                auto tile_view = make_interleaved_view<gil::gray8_pixel_t>(w, h, &dib[0], w);
                {
                    auto png = std::get<0>(t) + ".png";
                    benchmark bench{"write " + png};
                    gil::write_view(png, tile_view, gil::png_tag());
                }
                {
                    benchmark bench{"copy pixels from" + std::get<0>(t)};
                    gil::copy_pixels(
                        gil::subimage_view(tile_view, 0, 0, w, h),
                        gil::subimage_view(mosaic_view, x * w, y * h, w, h));
                }
            }
        }
        {
            auto png = tiles_dir + "\\mosaic-tiles.png";
            benchmark bench{ "write " + png };
            gil::write_view(png, mosaic_view, gil::png_tag());
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
