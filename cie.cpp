#include <algorithm>

struct BGRAQUAD
{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
};

int main()
{
    BGRAQUAD rgb[256] = { 0 };
    bool all = false;
    all = std::all_of(std::begin(rgb), std::end(rgb),
            [](auto const& c) -> bool {
                return c.rgbRed == 0 && c.rgbGreen == 0 && c.rgbBlue == 0 && c.rgbReserved == 0;
    });

    return 0;
}
