#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io_new/bmp_all.hpp>

using namespace std;
using namespace boost;
using namespace gil;

int main()
{
    typedef rgb8_image_t image_t;

    // read image using scanline_read_iterator
    const char* file_name = "D:\\dev\\gil\\_git\\boost-gil-workshop\\images\\g04.bmp";

    typedef scanline_reader
        <
        typename get_read_device<const char*, bmp_tag>::type, bmp_tag
        > reader_t;

    reader_t reader = make_scanline_reader( file_name, bmp_tag() );
    image_t dst( reader._info._width, reader._info._height );
    byte_t* buffer = new byte_t[ reader._scanline_length ];
    scanline_read_iterator<reader_t> it  = scanline_read_iterator<reader_t>(reader, buffer);
    scanline_read_iterator<reader_t> end = scanline_read_iterator<reader_t>();
    int row = 0;
    while( it != end )
    {
        *it; // ?

        copy_pixels(
            interleaved_view(reader._info._width, 1,
                (image_t::view_t::x_iterator) buffer, reader._scanline_length)
            , subimage_view(view(dst), 0, row, reader._info._width, 1));

        ++row;
    }

    return 0;
}