#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#pragma warning(disable:4244)
#pragma warning(disable:4503)
#pragma warning(disable:4996)
#include <iostream>
#include <typeinfo>
#include <boost/gil.hpp>
namespace gil = boost::gil;

int main()
{
    {
        using v_t = gil::gray8_image_t::view_t::value_type;
        std::cout << typeid(v_t).name() << std::endl;
        std::cout << sizeof(v_t) << std::endl;
    }
    std::cout << "\nbit-aligned 1\n" << std::endl;
    {
        using i_t = gil::bit_aligned_image1_type<1, gil::gray_layout_t>::type;
        using v_t = i_t::view_t::value_type;
        std::cout << typeid(v_t).name() << std::endl;
        std::cout << sizeof(v_t) << std::endl;
        std::cout << std::endl;
        using r_t = i_t::view_t::reference;
        std::cout << typeid(r_t).name() << std::endl;
        std::cout << sizeof(r_t) << std::endl;
    }
    std::cout << "\nbit-aligned 222\n" << std::endl;
    {
        using i_t = gil::bit_aligned_image3_type<2, 2, 2, gil::rgb_layout_t>::type;
        using v_t = i_t::view_t::value_type;
        std::cout << typeid(v_t).name() << std::endl;
        std::cout << sizeof(v_t) << std::endl;
        std::cout << std::endl;
        using r_t = i_t::view_t::reference;
        std::cout << typeid(r_t).name() << std::endl;
        std::cout << sizeof(r_t) << std::endl;
    }
    std::cout << "\nbit-aligned 565\n" << std::endl;
    {
        using i_t = gil::bit_aligned_image3_type<5, 6, 5, gil::rgb_layout_t>::type;
        using v_t = i_t::view_t::value_type;
        std::cout << typeid(v_t).name() << std::endl;
        std::cout << sizeof(v_t) << std::endl;
        std::cout << std::endl;
        using r_t = i_t::view_t::reference;
        std::cout << typeid(r_t).name() << std::endl;
        std::cout << sizeof(r_t) << std::endl;
    }
    std::cout << "\npacked 1\n" << std::endl;
    {
        using i_t = gil::packed_image1_type<unsigned char, 1, gil::gray_layout_t>::type;
        using v_t = i_t::view_t::value_type;
        std::cout << typeid(v_t).name() << std::endl;
        std::cout << sizeof(v_t) << std::endl;
        std::cout << std::endl;
        using r_t = i_t::view_t::reference;
        std::cout << typeid(r_t).name() << std::endl;
        std::cout << sizeof(r_t) << std::endl;
    }
    std::cout << "\npacked 222\n" << std::endl;
    {
        using i_t = gil::packed_image3_type<unsigned char, 2, 2, 2, gil::rgb_layout_t>::type;
        using v_t = i_t::view_t::value_type;
        std::cout << typeid(v_t).name() << std::endl;
        std::cout << sizeof(v_t) << std::endl;
        std::cout << std::endl;
        using r_t = i_t::view_t::reference;
        std::cout << typeid(r_t).name() << std::endl;
        std::cout << sizeof(r_t) << std::endl;
    }
    std::cout << "\npacked 565\n" << std::endl;
    {
        using i_t = gil::packed_image3_type<unsigned short, 5, 6, 5, gil::rgb_layout_t>::type;
        using v_t = i_t::view_t::value_type;
        std::cout << typeid(v_t).name() << std::endl;
        std::cout << sizeof(v_t) << std::endl;
        std::cout << std::endl;
        using r_t = i_t::view_t::reference;
        std::cout << typeid(r_t).name() << std::endl;
        std::cout << sizeof(r_t) << std::endl;
    }
}
