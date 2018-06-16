#include "promote_integral.hpp"
#include <cstdint>
#include <limits>
#include <iostream>

template <typename C>
inline C channel_invert1(C x)
{
    using promoted_t = typename boost::gil::promote_integral<C>::type;
    promoted_t const promoted_max = std::numeric_limits<C>::max();
    promoted_t const promoted_min = std::numeric_limits<C>::min();
    promoted_t const promoted_val = x;
    return promoted_max - promoted_val + promoted_min;
}

template <typename C>
inline C channel_invert2(C x)
{
    using promoted_t = typename boost::gil::promote_integral<C>::type;
    promoted_t const promoted_max = std::numeric_limits<C>::max();
    promoted_t const promoted_min = std::numeric_limits<C>::min();
    promoted_t const promoted_val = x;
    return (-1) * (promoted_val - promoted_max) + promoted_min;
}

int main()
{
    using C = std::int32_t;
    {
        auto x = std::numeric_limits<C>::min();
        std::cout << "channel min: " << x << std::endl;
        // plain expressions
        auto x_invert1 = std::numeric_limits<C>::max() - x + std::numeric_limits<C>::min();
        auto x_invert2 = (x - std::numeric_limits<C>::max()) * (-1) + std::numeric_limits<C>::min();
        std::cout << "channel inv1: " << x_invert1 << std::endl;
        std::cout << "channel inv2: " << x_invert2 << std::endl;

        // the same expressions wrapped in function template
        std::cout << "channel_invert1: " << channel_invert1<C>(x) << std::endl;
        std::cout << "channel_invert2: " << channel_invert2<C>(x) << std::endl;
    }
    {
        auto x = std::numeric_limits<C>::max();
        std::cout << "channel max: " << x << std::endl;
        // plain expressions
        auto x_invert1 = std::numeric_limits<C>::max() - x + std::numeric_limits<C>::min();
        auto x_invert2 = (x - std::numeric_limits<C>::max()) * (-1) + std::numeric_limits<C>::min();
        std::cout << "channel inv1: " << x_invert1 << std::endl;
        std::cout << "channel inv2: " << x_invert2 << std::endl;

        // the same expressions wrapped in function template
        std::cout << "channel_invert1: " << channel_invert1<C>(x) << std::endl;
        std::cout << "channel_invert2: " << channel_invert2<C>(x) << std::endl;
    }
}
