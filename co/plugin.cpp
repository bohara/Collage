
/* Copyright (c) 2009-2010, Cedric Stalder <cedric.stalder@gmail.com>
 *               2009-2012, Stefan Eilemann <eile@equalizergraphics.com>
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

#include "plugin.h"

#include "compressorInfo.h"
#include "global.h"
#include "log.h"
#include "pluginRegistry.h"

#include <cmath>

namespace co
{
bool Plugin::init( const std::string& libraryName )
{
    if( !_dso.open( libraryName ))
        return false;

    getNumCompressors = ( GetNumCompressors_t )
        ( _dso.getFunctionPointer( "EqCompressorGetNumCompressors" ));

    GetInfo_t getInfo = ( GetInfo_t )
        ( _dso.getFunctionPointer( "EqCompressorGetInfo" ));

    compress = ( Compress_t )
        ( _dso.getFunctionPointer( "EqCompressorCompress" ));

    decompress = ( Decompress_t )
        ( _dso.getFunctionPointer( "EqCompressorDecompress" ));

    getNumResults = ( GetNumResults_t )
        ( _dso.getFunctionPointer( "EqCompressorGetNumResults" ));

    getResult = ( GetResult_t )
        ( _dso.getFunctionPointer( "EqCompressorGetResult" ));

    deleteDecompressor = ( DeleteDecompressor_t )
        ( _dso.getFunctionPointer( "EqCompressorDeleteDecompressor" ));

    deleteCompressor = ( DeleteCompressor_t )
        ( _dso.getFunctionPointer( "EqCompressorDeleteCompressor" ));

    newCompressor = ( NewCompressor_t )
        ( _dso.getFunctionPointer( "EqCompressorNewCompressor" ));

    newDecompressor = ( NewDecompressor_t )
        ( _dso.getFunctionPointer( "EqCompressorNewDecompressor" ));

    isCompatible = ( IsCompatible_t )
        ( _dso.getFunctionPointer( "EqCompressorIsCompatible" ));

    download = ( Download_t )
        ( _dso.getFunctionPointer( "EqCompressorDownload" ));

    startDownload = ( StartDownload_t )
        ( _dso.getFunctionPointer( "EqCompressorStartDownload" ));

    finishDownload = ( FinishDownload_t )
        ( _dso.getFunctionPointer( "EqCompressorFinishDownload" ));

    upload = ( Upload_t )
        ( _dso.getFunctionPointer( "EqCompressorUpload" ));

    const bool foundBase = newDecompressor && newCompressor &&
        deleteCompressor && deleteDecompressor && getInfo && getNumCompressors;
    const bool foundCPU = getResult && getNumResults && decompress && compress;
    const bool foundGPU = isCompatible && download && upload;

    if( !foundBase || ( !foundCPU && !foundGPU ))
    {
        LBWARN << "Initializing compression DSO " << libraryName
           << " failed, at least one entry point missing" << std::endl;
        return false;
    }

    const size_t nCompressors = getNumCompressors();
    if( nCompressors == 0 )
    {
        LBWARN << "Initializing compression DSO " << libraryName
           << " failed, 0 compression engines reported" << std::endl;
        return false;
    }

    _infos.resize( nCompressors );

    for( size_t i = 0; i < nCompressors; ++i )
    {
        CompressorInfo& info = _infos[ i ];
        info.version = EQ_COMPRESSOR_VERSION;
        info.outputTokenType = EQ_COMPRESSOR_DATATYPE_NONE;
        info.outputTokenSize = 0;
        getInfo( i, &info );

        if(( info.capabilities & EQ_COMPRESSOR_USE_ASYNC_DOWNLOAD ) &&
            ( !startDownload || !finishDownload ))
        {
            LBWARN << "Download plugin claims to support async readback " <<
                      "but corresponding functions are missing" << std::endl;
            _infos.clear();
            return false;
        }
        info.ratingAlpha = powf( info.speed, .3f ) / info.ratio;
        info.ratingNoAlpha = info.ratingAlpha;

        if( info.capabilities & EQ_COMPRESSOR_IGNORE_ALPHA )
        {
            switch( info.tokenType )
            {
              case EQ_COMPRESSOR_DATATYPE_4_BYTE:
              case EQ_COMPRESSOR_DATATYPE_4_HALF_FLOAT:
              case EQ_COMPRESSOR_DATATYPE_4_FLOAT:
                  info.ratingNoAlpha /= .75f;
                  break;

              case EQ_COMPRESSOR_DATATYPE_RGB10_A2:
                  info.ratingNoAlpha /= .9375f; // 30/32
                  break;

              default:
                  break;
            }
        }

        if( !( info.capabilities & EQ_COMPRESSOR_TRANSFER ))
        {
            if( info.outputTokenType == EQ_COMPRESSOR_DATATYPE_NONE )
            {
                // Set up CPU compressor output to be input type
                info.outputTokenType = info.tokenType;
                LBASSERT( info.outputTokenSize == 0 );
            }
            else
            {
                LBASSERT( info.outputTokenSize != 0 );
            }
        }
#ifndef NDEBUG // Check that each compressor exist once
        for( size_t j = 0; j < i; ++j )
        {
            LBASSERTINFO( info.name != _infos[j].name,
                          "Compressors " << i << " and " << j << " in '" <<
                          libraryName << "' use the same name" );
        }
#endif

    }

    return true;
}

void Plugin::exit()
{
    _dso.close();
    _infos.clear();

    newCompressor = 0;
    newDecompressor = 0;
    deleteCompressor = 0;
    deleteDecompressor = 0;
    compress = 0;
    decompress = 0;
    getNumResults = 0;
    getNumCompressors = 0;
    getResult = 0;

}

void Plugin::initChildren()
{
    PluginRegistry& registry = Global::getPluginRegistry();
    const Plugins& plugins = registry.getPlugins();

    for( CompressorInfos::iterator i = _infos.begin(); i != _infos.end(); ++i )
    {
        CompressorInfo& info = *i;
        LBLOG( LOG_PLUGIN ) << lunchbox::disableFlush << "Engine 0x" << std::hex
                            << info.name;

        if( info.capabilities & EQ_COMPRESSOR_TRANSFER )
        {
            LBLOG( LOG_PLUGIN ) << " compressors:";
            // Find compressors for downloader
            for( Plugins::const_iterator j = plugins.begin();
                 j != plugins.end(); ++j )
            {
                const Plugin* plugin = *j;
                const CompressorInfos& infos = plugin->getInfos();

                for( CompressorInfos::const_iterator k = infos.begin();
                     k != infos.end(); ++k )
                {
                    const CompressorInfo& child = *k;
                    if( child.capabilities & EQ_COMPRESSOR_TRANSFER ||
                        child.tokenType != info.outputTokenType )
                    {
                        continue;
                    }

                    info.compressors.push_back( &child );
                    LBLOG( LOG_PLUGIN ) << " 0x" << child.name;
                }
            }
        }
        else
        {
            LBLOG( LOG_PLUGIN ) << " uploaders:";
            // Find uploaders for decompressor
            for( Plugins::const_iterator j = plugins.begin();
                 j != plugins.end(); ++j )
            {
                const Plugin* plugin = *j;
                const CompressorInfos& infos = plugin->getInfos();

                for( CompressorInfos::const_iterator k = infos.begin();
                     k != infos.end(); ++k )
                {
                    const CompressorInfo& child = *k;
                    if( !(child.capabilities & EQ_COMPRESSOR_TRANSFER) ||
                        child.tokenType != info.outputTokenType )
                    {
                        continue;
                    }

                    info.uploaders.push_back( &child );
                    LBLOG( LOG_PLUGIN ) << " 0x" << child.name;
                }
            }
        }
        LBLOG( LOG_PLUGIN ) << std::endl << std::dec << lunchbox::enableFlush;
    }
}


bool Plugin::implementsType( const uint32_t name ) const
{
    for( CompressorInfos::const_iterator i = _infos.begin();
         i != _infos.end(); ++i )
    {
        if ( i->name == name )
            return true;
    }

    return false;
}

const CompressorInfo& Plugin::findInfo( const uint32_t name ) const
{
    for( CompressorInfos::const_iterator i = _infos.begin();
         i != _infos.end(); ++i )
    {
        if( i->name == name )
            return (*i);
    }

    LBUNREACHABLE;
    return _infos.front();
}

}
