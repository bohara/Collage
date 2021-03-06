
/* Copyright (c) 2012, Daniel Nachbaur <danielnachbaur@gmail.com>
 *               2012, Stefan.Eilemann@epfl.ch
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "buffer.h"
#include "bufferListener.h"

namespace co
{
namespace detail
{
class Buffer
{
public:
    Buffer( BufferListener* listener_ )
            : listener( listener_ )
    {}

    BufferListener* const listener;
};
}

Buffer::Buffer( BufferListener* listener )
    : lunchbox::Bufferb()
    , lunchbox::Referenced()
    , _impl( new detail::Buffer( listener ))
{
}

Buffer::~Buffer()
{
    delete _impl;
}

void Buffer::deleteReferenced( const Referenced* object ) const
{
    Buffer* buffer = const_cast< Buffer* >( this );
    if( buffer->_impl->listener )
        buffer->_impl->listener->notifyFree( buffer );
}

size_t Buffer::getMinSize()
{
    return 256;
}

size_t Buffer::getCacheSize()
{
    return 4096; // Bigger than minSize!
}

std::ostream& operator << ( std::ostream& os, const Buffer& buffer )
{
    os << lunchbox::disableFlush << "Buffer[" << buffer.getRefCount() << "@"
       << &buffer << "]";
    buffer.printHolders( os );
    return os << lunchbox::enableFlush;
}
}
