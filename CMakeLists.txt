CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT(viva)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
FIND_PACKAGE(wxWidgets COMPONENTS gl core base REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(LibConfig REQUIRED)
FIND_PACKAGE(GTK2 REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(Graphviz)

#
# The Paje Library (libpaje)
#
SET(LIBPAJE_HEADERS
  src/libpaje/PajeColor.h
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
  src/libpaje/PajeTraceController.h
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
  src/libpaje/PajeTraceController.cc
  src/libpaje/PajeComponent.cc
  src/libpaje/PajeType.cc
  src/libpaje/PajeContainer.cc
  src/libpaje/PajeColor.cc
  src/libpaje/PajeEntity.cc)
ADD_LIBRARY(paje SHARED ${LIBPAJE_SOURCES})
SET_TARGET_PROPERTIES(paje PROPERTIES
  # bump when new function are added in the library
  VERSION 1.0
  # bump when ABI is broken (symbols removed or parameters changed)
  SOVERSION 1)
# bump when the API change (new functions in the header file)
SET(LIBPAJE_APIVERSION 1)
INSTALL(TARGETS paje DESTINATION lib)
INSTALL(FILES ${LIBPAJE_HEADERS} DESTINATION include)

#
# The Tupi Library written in C (particle system)
#
SET(LIBTUPI_HEADERS
  src/libtupi/tupi.h)
SET(LIBTUPI_SOURCES
  src/libtupi/bary.c
  src/libtupi/basic.c
  src/libtupi/dynar.c
  src/libtupi/dict.c
  src/libtupi/particle.c
  src/libtupi/box.c
  src/libtupi/cell.c
  src/libtupi/layout.c
  src/libtupi/node.c)
ADD_LIBRARY(tupi SHARED ${LIBTUPI_SOURCES})
TARGET_LINK_LIBRARIES(tupi m)
SET_TARGET_PROPERTIES(tupi PROPERTIES
  # bump when new function are added in the library
  VERSION 1.0
  # bump when ABI is broken (symbols removed or parameters changed)
  SOVERSION 1)
# bump when the API change (new functions in the header file)
SET(LIBTUPI_APIVERSION 1)
INSTALL(TARGETS tupi DESTINATION lib)
INSTALL(FILES ${LIBTUPI_HEADERS} DESTINATION include)

#
# Paje Validate (checks if a trace file is valid or not)
# 
ADD_EXECUTABLE(pj_validate src/paje_validate/main.cc)
INCLUDE_DIRECTORIES (pj_validate ${PROJECT_SOURCE_DIR}/src/libpaje/)
TARGET_LINK_LIBRARIES(pj_validate paje)
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pj_validate DESTINATION bin)

#
# Paje Dump
#
ADD_EXECUTABLE(pj_dump src/paje_dump/main.cc)
INCLUDE_DIRECTORIES (pj_dump ${PROJECT_SOURCE_DIR}/src/libpaje/)
TARGET_LINK_LIBRARIES(pj_dump paje)
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pj_dump DESTINATION bin)

#
# viva executable
#
SET(VIVA_SRC
  src/viva/main.cc
  src/viva/basic.cc
  src/viva/loadingbar.cc
  src/viva/graphframe.cc
  src/viva/timesliceframe.cc
  src/viva/graphwindow.cc
  src/viva/timeslicewindow.cc
  src/viva/VivaGraph.cc
  src/viva/VivaNode.cc
  src/viva/TimeInterval.cc)
ADD_EXECUTABLE(viva ${VIVA_SRC})
include_directories(viva ${GTK2_INCLUDE_DIRS})
include_directories(viva ${Cairo_INCLUDE_DIRS})
include_directories(viva ${PROJECT_SOURCE_DIR}/src/libtupi/)
include(${wxWidgets_USE_FILE})
target_link_libraries(viva ${wxWidgets_LIBRARIES} ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${Cairo_LIBRARIES} ${GTK2_LIBRARIES} ${LIBCONFIG_LIBRARIES} tupi paje )
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/viva DESTINATION bin)

#
# For pkgconfig
#
ADD_SUBDIRECTORY(pkgconfig)