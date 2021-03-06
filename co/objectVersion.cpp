
/* Copyright (c) 2009-2011, Stefan Eilemann <eile@equalizergraphics.com> 
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

#include "objectVersion.h"

#include "object.h"

namespace co
{
/** Special object version values */
const uint128_t VERSION_NONE( 0, 0 );
const uint128_t VERSION_FIRST( 0, 1 );
const uint128_t VERSION_OLDEST( 0, 0xfffffffffffffffcull );
const uint128_t VERSION_NEXT( 0, 0xfffffffffffffffdull );
const uint128_t VERSION_INVALID( 0, 0xfffffffffffffffeull ) ;
const uint128_t VERSION_HEAD( 0, 0xffffffffffffffffull );

ObjectVersion ObjectVersion::NONE;

ObjectVersion::ObjectVersion()
        : identifier( UUID::ZERO ), version( VERSION_NONE )
{}

ObjectVersion::ObjectVersion( const UUID& id_, const uint128_t& version_ )
        : identifier( id_ ), version( version_ )
{}

ObjectVersion::ObjectVersion( const Object* object )
        : identifier( UUID::ZERO ), version( VERSION_NONE )
{
    if( object && object->isAttached( ))
    {
        identifier = object->getID();
        version = object->getVersion(); 
    }
}

ObjectVersion& ObjectVersion::operator = ( const Object* object )
{
    if( object )
    {
        identifier = object->getID();
        version = object->getVersion();
    }
    else
    {
        identifier = UUID::ZERO;
        version = VERSION_NONE;
    }

    return *this;
}

}
