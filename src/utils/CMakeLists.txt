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

# The Paje Utils Library (libpajeutils)
SET(UTILS_HEADERS PajeThreadReader.h)
SET(UTILS_SOURCES PajeThreadReader.cc)

FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtOpenGl)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
QT4_WRAP_CPP(UTILS_HEADERS_MOC ${UTILS_HEADERS})
INCLUDE(${QT_USE_FILE})
find_package(Boost)
if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()


ADD_DEFINITIONS(${QT_DEFINITIONS})
LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(STATIC_LINKING)
  #static
  INCLUDE_DIRECTORIES (paje_utils_library_static ${PROJECT_SOURCE_DIR}/src/libpaje/)
  ADD_LIBRARY(paje_utils_library_static STATIC ${UTILS_SOURCES} ${UTILS_HEADERS_MOC})
  TARGET_LINK_LIBRARIES(paje_utils_library_static ${QT_LIBRARIES})
  TARGET_LINK_LIBRARIES(paje_utils_library_static ${GLUT_glut_LIBRARY})
  SET_TARGET_PROPERTIES(paje_utils_library_static PROPERTIES OUTPUT_NAME pajeutils)
ELSE(STATIC_LINKING)
  #shared
  INCLUDE_DIRECTORIES (paje_utils_library ${PROJECT_SOURCE_DIR}/src/libpaje/)
  ADD_LIBRARY(paje_utils_library SHARED ${UTILS_SOURCES} ${UTILS_HEADERS_MOC})
  TARGET_LINK_LIBRARIES(paje_utils_library ${QT_LIBRARIES})
  TARGET_LINK_LIBRARIES(paje_utils_library ${GLUT_glut_LIBRARY})
  TARGET_LINK_LIBRARIES(paje_utils_library paje_library)
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
  CONFIGURE_FILE (libpajeutils.pc.cmake libpajeutils.pc @ONLY)
  INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libpajeutils.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
ENDIF(STATIC_LINKING)
