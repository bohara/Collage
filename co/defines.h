 
/* Copyright (c) 2010-2012, Stefan Eilemann <eile@eyescale.ch> 
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

#ifndef CO_DEFINES_H
#define CO_DEFINES_H

// Lunchbox includes before, the generated files use LB_ defines.
#include <lunchbox/lunchbox.h>

#ifdef __APPLE__
#  include <co/definesDarwin.h>
#endif
#ifdef __linux
#  include <co/definesLinux.h>
#endif
#ifdef _WIN32 //_MSC_VER
#  include <co/definesWin32.h>
#endif

#endif // CO_DEFINES_H
