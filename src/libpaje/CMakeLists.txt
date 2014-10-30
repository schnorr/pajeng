# This file is part of PajeNG
#
# PajeNG is free software: you can redistribute it and/or modify it
# under the terms of the GNU Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PajeNG is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Public License for more details.
#
# You should have received a copy of the GNU Public License
# along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# The Paje Library (libpaje)
SET(LIBPAJE_HEADERS
  PajeColor.h
  PajeException.h
  PajeProtocols.h
  PajeComponent.h
  PajeContainer.h
  PajeData.h
  PajeEntity.h
  PajeEventDecoder.h
  PajeEventDefinition.h
  PajeTraceEvent.h
  PajeEvent.h
  PajeFileReader.h
  PajeObject.h
  PajeSimulator.h
  PajeType.h
  PajeValue.h
  PajeDefinitions.h)
SET(LIBPAJE_SOURCES
  PajeEventDefinition.cc
  PajeTraceEvent.cc
  PajeEvent.cc
  PajeData.cc
  PajeFileReader.cc
  PajeObject.cc
  PajeEventDecoder.cc
  PajeSimulator.cc
  PajeSimulator+Queries.cc
  PajeSimulator+Commands.cc
  PajeComponent.cc
  PajeType.cc
  PajeValue.cc
  PajeEntity.cc
  PajeContainer.cc
  PajeColor.cc
  PajeException.cc
  PajeDefinitions.cc
)

FIND_PACKAGE(Boost REQUIRED)

IF(STATIC_LINKING)
  # static library
  ADD_LIBRARY(paje_library_static STATIC ${LIBPAJE_SOURCES})
  SET_TARGET_PROPERTIES(paje_library_static PROPERTIES OUTPUT_NAME paje)
ELSE(STATIC_LINKING)
  # shared library
  ADD_LIBRARY(paje_library SHARED ${LIBPAJE_SOURCES})
  INCLUDE_DIRECTORIES(paje_library ${Boost_INCLUDE_DIR})
  SET_TARGET_PROPERTIES(paje_library PROPERTIES OUTPUT_NAME paje)
  SET_TARGET_PROPERTIES(paje_library PROPERTIES
    # bump when new function are added in the library
    VERSION 1.0
    # bump when ABI is broken (symbols removed or parameters changed)
    SOVERSION 1)
  # bump when the API change (new functions in the header file)
  SET(LIBPAJE_APIVERSION 1)
  INSTALL(TARGETS paje_library DESTINATION ${LIB_INSTALL_DIR})
  INSTALL(FILES ${LIBPAJE_HEADERS} DESTINATION include)

  #pkgconfig
  CONFIGURE_FILE (libpaje.pc.cmake libpaje.pc @ONLY)
  INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libpaje.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
ENDIF(STATIC_LINKING)