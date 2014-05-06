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
PROJECT(pajeng)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(LIB_INSTALL_DIR lib${LIB_SUFFIX} CACHE PATH "Directory where libraries are installed (relative to CMAKE_INSTALL_PREFIX).")

# The PajeNG (Qt4 version)
SET(PAJE_HEADERS
  src/pajeng/PajeApplication.h
  src/pajeng/PajeWindow.h
  src/pajeng/PajeSpaceTimeFrame.h
  src/pajeng/PajeSpaceTimeView.h)
SET(PAJE_SOURCES
  src/pajeng/main.cc
  src/pajeng/PajeApplication.cc
  src/pajeng/PajeWindow.cc
  src/pajeng/PajeSpaceTimeFrame.cc
  src/pajeng/PajeSpaceTimeView.cc
  src/pajeng/STEntityTypeLayout.cc
  src/pajeng/PajeGraphicsItem.cc)

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
  src/libpaje/PajeValue.h)
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
)

# The Paje Utils Library (libpajeutils)
SET(UTILS_HEADERS
  src/utils/PajeThreadReader.h)
SET(UTILS_SOURCES
  src/utils/PajeThreadReader.cc)

# Paje Validate (checks if a trace file is valid or not)
SET(PAJEVALIDATE_SOURCES src/pj_validate/pj_validate.cc)

# Paje Dump
SET(PAJEDUMP_SOURCES src/pj_dump/pj_dump.cc)

# the cmake options
OPTION(PAJENG         "The PajeNG visualization tool (Qt4 version)" ON)
OPTION(PAJE_UTILS_LIBRARY "The Paje Utils library with QT-dependent components" ON)
OPTION(PAJE_LIBRARY "The Paje library with all the basic components" ON)
OPTION(PJ_DUMP    "Compile pj_dump (program to dump the contents of a trace" ON)
OPTION(PJ_VALIDATE "Compile pj_validate (program to validate a trace" ON)

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
  INSTALL(TARGETS paje_library DESTINATION ${LIB_INSTALL_DIR})
  INSTALL(FILES ${LIBPAJE_HEADERS} DESTINATION include)

  #pkgconfig
  CONFIGURE_FILE (src/libpaje/libpaje.pc.cmake libpaje.pc @ONLY)
  INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libpaje.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
ENDIF(PAJE_LIBRARY)

#
# The Paje Utils Library (libpajeutils)
#
IF(PAJE_UTILS_LIBRARY)
  FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtOpenGl)
  FIND_PACKAGE(GLUT REQUIRED)
  FIND_PACKAGE(OpenGL REQUIRED)
  QT4_WRAP_CPP(UTILS_HEADERS_MOC ${UTILS_HEADERS})
  INCLUDE(${QT_USE_FILE})
  ADD_DEFINITIONS(${QT_DEFINITIONS})
  LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

  ADD_LIBRARY(paje_utils_library SHARED ${UTILS_SOURCES} ${UTILS_HEADERS_MOC})
  TARGET_LINK_LIBRARIES(paje_utils_library ${QT_LIBRARIES})
  TARGET_LINK_LIBRARIES(paje_utils_library ${GLUT_glut_LIBRARY})
  TARGET_LINK_LIBRARIES(paje_utils_library paje)

  SET_TARGET_PROPERTIES(paje_utils_library PROPERTIES OUTPUT_NAME pajeutils)
  SET_TARGET_PROPERTIES(paje_utils_library PROPERTIES
    # bump when new function are added in the library
    VERSION 1.0
    # bump when ABI is broken (symbols removed or parameters changed)
    SOVERSION 1)
  # bump when the API change (new functions in the header file)
  SET(LIBPAJEUTILS_APIVERSION 1)
  INSTALL(TARGETS paje_utils_library DESTINATION ${LIB_INSTALL_DIR})
  INSTALL(FILES ${UTILS_HEADERS} DESTINATION include)

  #pkgconfig
  CONFIGURE_FILE (src/utils/libpajeutils.pc.cmake libpajeutils.pc @ONLY)
  INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libpajeutils.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
ENDIF(PAJE_UTILS_LIBRARY)

#
# Paje Validate (checks if a trace file is valid or not)
#
IF(PJ_VALIDATE)
  IF(NOT PAJE_LIBRARY)
    MESSAGE(FATAL_ERROR "You must enable PAJE_LIBRARY to compile pj_validate")
  ENDIF(NOT PAJE_LIBRARY)
  ADD_EXECUTABLE(pj_validate ${PAJEVALIDATE_SOURCES})
  INCLUDE_DIRECTORIES (pj_validate ${PROJECT_SOURCE_DIR}/src/libpaje/)
  TARGET_LINK_LIBRARIES(pj_validate paje_library)
  INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pj_validate DESTINATION bin)
ENDIF(PJ_VALIDATE)

# Paje Dump
IF(PJ_DUMP)
  IF(NOT PAJE_LIBRARY)
    MESSAGE(FATAL_ERROR "You must enable PAJE_LIBRARY to compile pj_dump")
  ENDIF(NOT PAJE_LIBRARY)
  ADD_EXECUTABLE(pj_dump ${PAJEDUMP_SOURCES})
  INCLUDE_DIRECTORIES (pj_dump ${PROJECT_SOURCE_DIR}/src/libpaje/)
  TARGET_LINK_LIBRARIES(pj_dump paje_library)
  INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pj_dump DESTINATION bin)
ENDIF(PJ_DUMP)

# The PajeNG
IF(PAJENG)
  IF(NOT PAJE_LIBRARY)
    MESSAGE(FATAL_ERROR "You must enable PAJE_LIBRARY to compile pajeng")
  ENDIF(NOT PAJE_LIBRARY)
  IF(NOT PAJE_UTILS_LIBRARY)
    MESSAGE(FATAL_ERROR "You must enable PAJE_UTILS_LIBRARY to compile pajeng")
  ENDIF(NOT PAJE_UTILS_LIBRARY)
  FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtOpenGl)
  FIND_PACKAGE(GLUT REQUIRED)
  FIND_PACKAGE(OpenGL REQUIRED)
  QT4_WRAP_CPP(PAJE_HEADERS_MOC ${PAJE_HEADERS})
  INCLUDE(${QT_USE_FILE})
  ADD_DEFINITIONS(${QT_DEFINITIONS})
  ADD_EXECUTABLE(pajeng ${PAJE_SOURCES} ${PAJE_HEADERS_MOC})
  INCLUDE_DIRECTORIES(pajeng ${PROJECT_SOURCE_DIR}/src/libpaje/)
  TARGET_LINK_LIBRARIES(pajeng ${QT_LIBRARIES})
  TARGET_LINK_LIBRARIES(pajeng ${GLUT_glut_LIBRARY})
  TARGET_LINK_LIBRARIES(pajeng ${OPENGL_gl_LIBRARY})
  TARGET_LINK_LIBRARIES(pajeng ${OPENGL_glu_LIBRARY})
  TARGET_LINK_LIBRARIES(pajeng paje_library)
  TARGET_LINK_LIBRARIES(pajeng paje_utils_library)
  INSTALL(PROGRAMS ${pajeng_BINARY_DIR}/pajeng DESTINATION bin)
  INSTALL(FILES ${UTILS_HEADERS} DESTINATION include)
ENDIF(PAJENG)

add_subdirectory(doc)