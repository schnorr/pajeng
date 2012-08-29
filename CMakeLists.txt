# This file is part of PajeNG
#
# PajeNG is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PajeNG is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser Public License for more details.
#
# You should have received a copy of the GNU Lesser Public License
# along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT(pajeng)

# Utils
SET(UTILS_HEADERS
  src/utils/PajeThreadReader.h)
SET(UTILS_SOURCES
  src/utils/PajeThreadReader.cc)

# The PajeNG (Qt4 version)
SET(PAJE_HEADERS
  src/paje/PajeApplication.h
  src/paje/PajeWindow.h
  src/paje/PajeSpaceTimeFrame.h
  src/paje/PajeSpaceTimeView.h
  src/paje/PajeTreemapFrame.h
  src/paje/PajeTreemapView.h)
SET(PAJE_SOURCES
  src/paje/main.cc
  src/paje/PajeApplication.cc
  src/paje/PajeWindow.cc
  src/paje/PajeSpaceTimeFrame.cc
  src/paje/PajeSpaceTimeView.cc
  src/paje/PajeTreemapFrame.cc
  src/paje/PajeTreemapView.cc
  src/paje/PajeTreemap.cc
  src/paje/STEntityTypeLayout.cc
  src/paje/PajeGraphicsItem.cc
  src/paje/PajeTreemapItem.cc)

# The Paje Library (libpaje)
SET(LIBPAJE_HEADERS
  src/libpaje/PajeColor.h
  src/libpaje/PajeProtocols.h
  src/libpaje/PajeComponent.h
  src/libpaje/PajeContainer.h
  src/libpaje/PajeData.h
  src/libpaje/PajeEntity.h
  src/libpaje/PajeEventDecoder.h
  src/libpaje/PajeEventDefinition.h
  src/libpaje/PajeEvent.h
  src/libpaje/PajeFileReader.h
  src/libpaje/PajeObject.h
  src/libpaje/PajeSimulator.h
  src/libpaje/PajeType.h)
SET(LIBPAJE_SOURCES
  src/libpaje/PajeEventDefinition.cc
  src/libpaje/PajeEvent.cc
  src/libpaje/PajeData.cc
  src/libpaje/PajeFileReader.cc
  src/libpaje/PajeEventDecoder.cc
  src/libpaje/PajeSimulator.cc
  src/libpaje/PajeSimulator+Queries.cc
  src/libpaje/PajeSimulator+Commands.cc
  src/libpaje/PajeComponent.cc
  src/libpaje/PajeType.cc
  src/libpaje/PajeContainer.cc
  src/libpaje/PajeColor.cc
  src/libpaje/PajeEntity.cc)

# Paje Validate (checks if a trace file is valid or not)
SET(PAJEVALIDATE_SOURCES src/paje_validate/main.cc)

# Paje Dump
SET(PAJEDUMP_SOURCES src/paje_dump/main.cc)

# the cmake options
OPTION(PAJE         "The PajeNG visualization tool (Qt4 version)" OFF)
OPTION(PAJE_LIBRARY "The Paje library with all the basic components" ON)
OPTION(PAJE_DUMP    "Compile pj_dump (program to dump the contents of a trace" ON)
OPTION(PAJE_VALIDATE "Compile pj_validate (program to validate a trace" ON)

# The Paje Library (libpaje) 
IF(PAJE_LIBRARY)
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
  INSTALL(TARGETS paje_library DESTINATION lib)
  INSTALL(FILES ${LIBPAJE_HEADERS} DESTINATION include)

  #pkgconfig
  CONFIGURE_FILE (src/libpaje/libpaje.pc.cmake libpaje.pc @ONLY)
  INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libpaje.pc DESTINATION lib/pkgconfig)
ENDIF(PAJE_LIBRARY)

# Paje Validate (checks if a trace file is valid or not)
IF(PAJE_VALIDATE)
  IF(NOT PAJE_LIBRARY)
    MESSAGE(FATAL_ERROR "You must enable PAJE_LIBRARY to compile pj_validate")
  ENDIF(NOT PAJE_LIBRARY)
  ADD_EXECUTABLE(pj_validate ${PAJEVALIDATE_SOURCES})
  INCLUDE_DIRECTORIES (pj_validate ${PROJECT_SOURCE_DIR}/src/libpaje/)
  TARGET_LINK_LIBRARIES(pj_validate paje_library)
  INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pj_validate DESTINATION bin)
ENDIF(PAJE_VALIDATE)

# Paje Dump
IF(PAJE_DUMP)
  IF(NOT PAJE_LIBRARY)
    MESSAGE(FATAL_ERROR "You must enable PAJE_LIBRARY to compile pj_dump")
  ENDIF(NOT PAJE_LIBRARY)
  ADD_EXECUTABLE(pj_dump ${PAJEDUMP_SOURCES})
  INCLUDE_DIRECTORIES (pj_dump ${PROJECT_SOURCE_DIR}/src/libpaje/)
  TARGET_LINK_LIBRARIES(pj_dump paje_library)
  INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pj_dump DESTINATION bin)
ENDIF(PAJE_DUMP)

# The PajeNG
IF(PAJE)
  FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtOpenGl)
  FIND_PACKAGE(GLUT REQUIRED)
  FIND_PACKAGE(OpenGL REQUIRED)
  QT4_WRAP_CPP(PAJE_HEADERS_MOC ${PAJE_HEADERS})
  QT4_WRAP_CPP(UTILS_HEADERS_MOC ${UTILS_HEADERS})
  INCLUDE(${QT_USE_FILE})
  ADD_DEFINITIONS(${QT_DEFINITIONS})
  ADD_EXECUTABLE(pajeng ${PAJE_SOURCES} ${PAJE_HEADERS_MOC} ${UTILS_SOURCES} ${UTILS_HEADERS_MOC})
  INCLUDE_DIRECTORIES(pajeng ${PROJECT_SOURCE_DIR}/src/libpaje/)
  TARGET_LINK_LIBRARIES(pajeng ${QT_LIBRARIES} ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} paje_library)
  INSTALL(PROGRAMS ${pajeng_BINARY_DIR}/pajeng DESTINATION bin)
ENDIF(PAJE)
