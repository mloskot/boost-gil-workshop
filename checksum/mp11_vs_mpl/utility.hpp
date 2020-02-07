#pragma once
#include <fstream>
#include <string>
#include <tuple>
#include <vector>

template <typename View>
auto dump(View const& view) -> std::vector<std::tuple<int, int, int>>
{
    std::vector<std::tuple<int, int, int>> v1;
    v1.reserve(view.width() * view.height());
    gil::for_each_pixel(view, [&v1](auto const& p) {
        auto b = (int)get_color(p, gil::blue_t{});
        auto g = (int)get_color(p, gil::green_t{});
        auto r = (int)get_color(p, gil::red_t{});
        v1.emplace_back(r, g, b);
    });
    return v1;
}

void save(std::vector<std::tuple<int, int, int>> const& v, std::string f)
{
    std::ofstream ofs(f);
    for (auto& p : v)
        ofs << std::get<0>(p) << '\t' << std::get<1>(p) << '\t' << std::get<1>(p) << "\t\n";
}
