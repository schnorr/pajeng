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

# The PajeNG (Qt4 version)
SET(PAJE_HEADERS
  PajeApplication.h
  PajeWindow.h
  PajeSpaceTimeFrame.h
  PajeSpaceTimeView.h)
SET(PAJE_SOURCES
  main.cc
  PajeApplication.cc
  PajeWindow.cc
  PajeSpaceTimeFrame.cc
  PajeSpaceTimeView.cc
  STEntityTypeLayout.cc
  PajeGraphicsItem.cc)

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
find_package(Boost)
if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()

INCLUDE_DIRECTORIES(pajeng ${PROJECT_SOURCE_DIR}/src/libpaje/)
TARGET_LINK_LIBRARIES(pajeng ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(pajeng ${GLUT_glut_LIBRARY})
TARGET_LINK_LIBRARIES(pajeng ${OPENGL_gl_LIBRARY})
TARGET_LINK_LIBRARIES(pajeng ${OPENGL_glu_LIBRARY})
IF(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pajeng paje_library_static)
  TARGET_LINK_LIBRARIES(pajeng paje_utils_library_static)
ELSE(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pajeng paje_library)
  TARGET_LINK_LIBRARIES(pajeng paje_utils_library)
ENDIF(STATIC_LINKING)
SET_PROPERTY(TARGET pajeng PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/pajeng DESTINATION bin)
INSTALL(FILES ${UTILS_HEADERS} DESTINATION include)
