
# Copyright (c) 2010 Daniel Pfeifer <daniel@pfeifer-mail.de>
#               2010-2012 Stefan Eilemann <eile@eyescale.ch>
#               2010 Cedric Stalder <cedric.stalder@gmail.ch>

include(configure.cmake)
include(files.cmake)
include(PurpleAddLibrary)

option(COLLAGE_AGGRESSIVE_CACHING "Disable to reduce memory consumption" ON)
mark_as_advanced(COLLAGE_AGGRESSIVE_CACHING)

add_definitions(-DEQ_PLUGIN_BUILD)

set(CO_ADD_LINKLIB ${PTHREAD_LIBRARIES} ${LUNCHBOX_LIBRARIES})

if(WIN32)
  list(APPEND CO_ADD_LINKLIB ws2_32 mswsock)
endif(WIN32)
if(LINUX)
  list(APPEND CO_ADD_LINKLIB dl rt)
endif()

if(Boost_FOUND)
  list(APPEND CO_SOURCES dataIStreamArchive.cpp dataOStreamArchive.cpp
                         rspConnection.cpp)
  list(APPEND CO_ADD_LINKLIB ${Boost_SERIALIZATION_LIBRARY}
                             ${Boost_SYSTEM_LIBRARY})
  if(NOT Boost_USE_STATIC_LIBS)
    list(APPEND CO_ADD_LINKLIB ${Boost_DATE_TIME_LIBRARY})
  endif()
  add_definitions(-DBOOST_ARCHIVE_SOURCE)
endif(Boost_FOUND)

if(OFED_FOUND)
  include_directories(SYSTEM ${OFED_INCLUDE_DIRS})
  list(APPEND CO_HEADERS rdmaConnection.h)
  list(APPEND CO_SOURCES rdmaConnection.cpp)
  list(APPEND CO_ADD_LINKLIB ${OFED_LIBRARIES})
endif()

if(UDT_FOUND)
  include_directories(SYSTEM ${UDT_INCLUDE_DIRS})
  list(APPEND CO_HEADERS udtConnection.h)
  list(APPEND CO_SOURCES udtConnection.cpp)
  list(APPEND CO_ADD_LINKLIB ${UDT_LIBRARIES})
endif()

source_group(\\ FILES CMakeLists.txt)
source_group(plugin FILES ${PLUGIN_HEADERS} )
source_group(collage FILES ${CO_PUBLIC_HEADERS} ${CO_HEADERS} ${CO_SOURCES} )
source_group(compressor FILES ${CO_COMPRESSORS})

purple_add_library(Collage SHARED
  HEADERS_PREFIX "co/"
  HEADERS ${CO_PUBLIC_HEADERS} ${PLUGIN_HEADERS}
  SOURCES ${CO_HEADERS} ${CO_SOURCES} ${CO_COMPRESSORS}
  LINK_LIBRARIES ${CO_ADD_LINKLIB}
  )

# TODO: this will be incorporated into PURPLE_ADD_LIBRARY
update_file(Collage.pc ${CMAKE_CURRENT_BINARY_DIR}/Collage.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Collage.pc
        DESTINATION ${LIBRARY_DIR}/pkgconfig COMPONENT lib)
install(FILES ${OPTIONS_CMAKE} DESTINATION share/Collage/CMake COMPONENT dev)
