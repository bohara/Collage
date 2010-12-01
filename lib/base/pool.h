
/* Copyright (c) 2010, Stefan Eilemann <eile@eyescale.ch> 
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

#ifndef EQBASE_POOL_H
#define EQBASE_POOL_H

#include <eq/base/spinLock.h>     // member
#include <eq/base/nonCopyable.h>  // base class

namespace eq
{
namespace base
{
    /** An object allocation pool. */
    template< typename T, bool locked = false > class Pool : public NonCopyable
    {
    public:
        /** Construct a new pool. @version 1.0 */
        Pool() : _lock( locked ? new SpinLock : 0 ) {}

        /** Destruct this pool. @version 1.0 */
        virtual ~Pool() { flush(); delete _lock; }

        /** @return a reusable or new item. @version 1.0 */
        T* alloc()
            {
                ScopedMutex< SpinLock > mutex( _lock );
                if( _cache.empty( ))
                    return new T;

                T* item = _cache.back();
                _cache.pop_back();
                return item;
            }

        /** Release an item for reuse. @version 1.0 */
        void release( T* item )
            {
                ScopedMutex< SpinLock > mutex( _lock );
                _cache.push_back( item );
            }

        /** Delete all cached items. @version 1.0 */
        void flush()
            {
                ScopedMutex< SpinLock > mutex( _lock );
                while( !_cache.empty( ))
                {
                    delete _cache.back();
                    _cache.pop_back();
                }
            }

    private:
        SpinLock* const _lock;
        std::vector< T* > _cache;
    };
}
}
#endif // EQBASE_POOL_H