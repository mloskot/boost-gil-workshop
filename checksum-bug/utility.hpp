#pragma once
#include <boost/crc.hpp>
#include <boost/gil.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
namespace fs = std::filesystem;
namespace gil = boost::gil;

#ifdef NDEBUG
std::string file_suffix = "_r";
#else
std::string file_suffix = "_d";
#endif

template <typename View>
auto checksum(View const& img_view) -> std::string
{
    boost::crc_32_type crc;
    {
        gil::rgb8_image_t rgb_img(img_view.dimensions());
        gil::copy_and_convert_pixels(img_view, gil::view(rgb_img));
        auto rgb_view = gil::view(rgb_img);
        crc.process_bytes(rgb_view.row_begin(0), rgb_view.size() * 3);
    }
    std::ostringstream oss;
    oss << std::hex << crc.checksum();
    return oss.str();
}

template <typename View>
auto dump(View const& view) -> std::tuple<std::string, std::vector<std::tuple<int, int, int>>>
{
    std::vector<std::tuple<int, int, int>> v1;
    v1.reserve(view.width() * view.height());
    gil::for_each_pixel(view, [&v1](auto const& p) {
        auto b = (int)get_color(p, gil::blue_t{});
        auto g = (int)get_color(p, gil::green_t{});
        auto r = (int)get_color(p, gil::red_t{});
        v1.emplace_back(r, g, b);
    });

    auto c = checksum(view);
    return std::make_tuple(c, v1);
}

void save(std::tuple<std::string, std::vector<std::tuple<int, int, int>>> const& d, std::string f)
{
    fs::path dir_name = fs::current_path().stem();
    fs::path dir_path = fs::current_path().parent_path().parent_path().parent_path() / dir_name;
    auto out_path  = dir_path / (f + file_suffix + ".txt");
    std::ofstream ofs(out_path);

    auto& [c, v] = d;
    ofs << c << std::endl;
    for (auto& p : v)
        ofs << std::get<0>(p) << '\t' << std::get<1>(p) << '\t' << std::get<1>(p) << "\t\n";
    ofs << std::endl;
}

template <typename View>
void save_dump(View const& view, std::string filename)
{
    auto d0 = dump(view);
    save(d0, filename);
}
