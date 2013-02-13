#pragma once

namespace boost { namespace gil {

/// Input iterator to read images.
template< typename Reader >
class scanline_read_iterator
{
public:

    typedef typename Reader::backend_t backend_t;

    typedef std::input_iterator_tag iterator_category;
    typedef byte_t* value_type;
    typedef value_type const* pointer;
    typedef value_type const& reference;
    
    /// todo
    //typedef ptrdiff_t difference_type;

public:

    /// Default Constructor, usually used to create an end iterator.
    scanline_read_iterator()
    : _pos( -1 )
    , _reader( NULL )
    , _buffer( NULL )
    {}

    /// Constructor with preallocated image. Reading starts at first scanline of source image.
    scanline_read_iterator( Reader&       reader
                          , const byte_t* buffer
                          )
    : _pos( 0 )
    , _reader( &reader )
    , _buffer( buffer  )
    {
        init();
    }

    /// Constructor with preallocated image. Reading starts at first scanline of source image.
    scanline_read_iterator( Reader& reader )
    : _pos( 0 )
    , _reader( &reader )
    , _buffer( NULL )
    {
        init();
    }

    /// Constructor with preallocated image. Reading starts at pos scanline of source image.
    scanline_read_iterator( std::size_t pos
                          , Reader&     reader
                          , byte_t*     buffer
                          )
    : _pos( pos )
    , _reader( &reader )
    , _buffer( buffer  )
    {
        init();

        if( _pos >= _reader._info._height )
        {
            std::runtime_error( "Trying to read past image." );
        }

        if( pos > 0 && _buffer == null )
        {
            std::runtime_error( "Cannot proceed without initializing read buffer." );
        }

        for( std::size_t i = 0; i < pos; ++i )
        {
            _skip();
        }
    }

    //
    // Destructor
    //
    ~scanline_read_iterator()
    {
        if( _reader )
        {
            _reader->clean_up();
        }
    }

    /// Set reader. Do clean up before if necessary.
    void set_reader( Reader& reader )
    {
        if( _reader )
        {
            _reader->clean_up();

            _pos = 0;
        }

        _reader = &reader;

        init();
    }

    /// Set reader. Do clean up before if necessary.
    void set_buffer( byte_t* buffer )
    {
        _buffer = buffer;
    }


    /// Dereference Operator
    reference operator*() const
    {
        if( _reader && _buffer )
        {
            _reader->read( _buffer, _pos );

            ++_pos;

            return _buffer;
        }

        throw std::runtime_error( "Reader cannot be null for this operation." );
    }

    /// Pointer-To-Memper Operator.
    pointer operator->() const
    {
        return &(operator*());
    }

    /// Pre-Increment Operator
    scanline_read_iterator< Reader >& operator++()
    {
        if( _buffer == NULL )
        {
            throw std::runtime_error( "Cannot proceed without initializing read buffer." );
        }

        _skip();

        return (*this);
    }

    /// Compare passed iterator to this.
    bool equal( const scanline_read_iterator< Reader >& rhs ) const
    {
        return (_reader == rhs._reader) && ( _buffer == rhs._buffer );
    }

    /// Return backend.
    typename const backend_t& backend()
    {
        if( _reader )
        {
            return *_reader;
        }

        throw std::runtime_error( "Reader cannot be null for this operation." );
    }

private:

    void init()
    {
        if( _reader )
        {
            _reader->read_header();
            _reader->initialize();
        }
    }

    void _skip()
    {
        if( _reader )
        {
            _reader->skip( _buffer, _pos );

            ++_pos;
        }
    }

private:

    mutable int _pos;

    Reader* _reader;
    byte_t* _buffer;
};

} // namespace gil
} // namespace boost
