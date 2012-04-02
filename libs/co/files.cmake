
# Copyright (c) 2010 Cedric Stalder <cedric.stalder@gmail.ch>
#               2011-2012 Stefan Eilemann <eile@eyescale.ch>

set(CO_PUBLIC_HEADERS 
    api.h
    barrier.h
    bufferConnection.h
    co.h
    command.h
    commandCache.h
    commandFunc.h
    commandQueue.h
    commands.h
    connection.h
    connection.ipp
    connectionDescription.h
    connectionListener.h
    connectionSet.h
    connectionType.h
    dataIStream.h
    dataIStreamArchive.h
    dataOStream.h
    dataOStreamArchive.h
    defines.h
    dispatcher.h
    error.h
    errorRegistry.h
    exception.h
    global.h
    init.h
    instanceCache.h
    localNode.h
    log.h
    node.h
    nodeType.h
    object.h
    objectVersion.h
    packets.h
    pluginRegistry.h
    queueMaster.h
    queuePackets.h
    queueSlave.h
    serializable.h
    types.h
    worker.h
  )

set(CO_HEADERS 
    barrierPackets.h
    compressor.h
    compressorInfo.h
    cpuCompressor.h
    dataIStreamQueue.h
    dataOStream.ipp
    deltaMasterCM.h
    error.cpp
    errorRegistry.cpp
    eventConnection.h
    fullMasterCM.h
    masterCM.h
    nodePackets.h
    nullCM.h
    objectCM.h
    objectDataIStream.h
    objectDataOStream.h
    objectDeltaDataOStream.h
    objectInstanceDataOStream.h
    objectSlaveDataOStream.h
    pgmConnection.h
    pipeConnection.h
    plugin.h
    rspConnection.h
    socketConnection.h
    staticMasterCM.h
    staticSlaveCM.h
    unbufferedMasterCM.h
    versionedSlaveCM.h
  )

set(CO_SOURCES
    barrier.cpp
    bufferConnection.cpp
    command.cpp
    commandCache.cpp
    commandQueue.cpp
    compressor.cpp
    connection.cpp
    connectionDescription.cpp
    connectionSet.cpp
    cpuCompressor.cpp
    dataIStream.cpp
    dataIStreamQueue.cpp
    dataOStream.cpp
    deltaMasterCM.cpp
    dispatcher.cpp
    eventConnection.cpp
    fullMasterCM.cpp
    global.cpp
    init.cpp
    instanceCache.cpp
    localNode.cpp
    masterCM.cpp
    node.cpp
    object.cpp
    objectCM.cpp
    objectDataIStream.cpp
    objectDataOStream.cpp
    objectDeltaDataOStream.cpp
    objectInstanceDataOStream.cpp
    objectSlaveDataOStream.cpp
    objectStore.cpp
    objectVersion.cpp
    packets.cpp
    pgmConnection.cpp
    pipeConnection.cpp
    plugin.cpp
    pluginRegistry.cpp
    queueMaster.cpp
    queueSlave.cpp
    socketConnection.cpp
    staticSlaveCM.cpp
    unbufferedMasterCM.cpp
    version.cpp
    versionedSlaveCM.cpp
    worker.cpp
)
  
set(CO_COMPRESSOR_HEADERS
    compressor/compressor.h
    compressor/compressorRLE4B.h
    compressor/compressorRLE4BU.h
    compressor/compressorRLE4HF.h
    compressor/compressorRLE10A2.h
    compressor/compressorRLE565.h
    compressor/compressorRLEB.h
    compressor/compressorRLEYUV.h
)
  
set(CO_COMPRESSOR_SOURCES
    compressor/compressor.cpp
    compressor/compressorRLE.ipp
    compressor/compressorRLE4B.cpp
    compressor/compressorRLE4BU.cpp
    compressor/compressorRLE4HF.cpp
    compressor/compressorRLE10A2.cpp
    compressor/compressorRLE565.cpp
    compressor/compressorRLEB.cpp
    compressor/compressorRLEYUV.cpp
)

set(PLUGIN_HEADERS
    plugins/compressor.h
    plugins/compressorTokens.h
    plugins/compressorTypes.h
)

if(WIN32)
  list(APPEND CO_HEADERS namedPipeConnection.h)
  list(APPEND CO_SOURCES namedPipeConnection.cpp)
else()
  list(APPEND CO_HEADERS fdConnection.h)
  list(APPEND CO_SOURCES fdConnection.cpp)
endif()
