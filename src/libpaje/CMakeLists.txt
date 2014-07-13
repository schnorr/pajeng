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
  src/libpaje/PajeColor.h
  src/libpaje/PajeException.h
  src/libpaje/PajeProtocols.h
  src/libpaje/PajeComponent.h
  src/libpaje/PajeContainer.h
  src/libpaje/PajeData.h
  src/libpaje/PajeEntity.h
  src/libpaje/PajeEventDecoder.h
  src/libpaje/PajeEventDefinition.h
  src/libpaje/PajeTraceEvent.h
  src/libpaje/PajeEvent.h
  src/libpaje/PajeFileReader.h
  src/libpaje/PajeObject.h
  src/libpaje/PajeSimulator.h
  src/libpaje/PajeType.h
  src/libpaje/PajeValue.h
  src/libpaje/PajeDefinitions.h)
SET(LIBPAJE_SOURCES
  src/libpaje/PajeEventDefinition.cc
  src/libpaje/PajeTraceEvent.cc
  src/libpaje/PajeEvent.cc
  src/libpaje/PajeData.cc
  src/libpaje/PajeFileReader.cc
  src/libpaje/PajeObject.cc
  src/libpaje/PajeEventDecoder.cc
  src/libpaje/PajeSimulator.cc
  src/libpaje/PajeSimulator+Queries.cc
  src/libpaje/PajeSimulator+Commands.cc
  src/libpaje/PajeComponent.cc
  src/libpaje/PajeType.cc
  src/libpaje/PajeValue.cc
  src/libpaje/PajeEntity.cc
  src/libpaje/PajeContainer.cc
  src/libpaje/PajeColor.cc
  src/libpaje/PajeException.cc
  src/libpaje/PajeDefinitions.cc
)

FIND_PACKAGE(Boost REQUIRED)
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
CONFIGURE_FILE (src/libpaje/libpaje.pc.cmake libpaje.pc @ONLY)
INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libpaje.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
