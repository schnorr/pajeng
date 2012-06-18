CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT(viva)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
FIND_PACKAGE(wxWidgets COMPONENTS gl core base REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(LibConfig REQUIRED)
FIND_PACKAGE(GTK2 REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(Cairo)
FIND_PACKAGE(Graphviz)

SET(CMAKE_CXX_FLAGS "-g -rdynamic")

#
# The Paje Library (libpaje)
#
SET(LIBPAJE_HEADERS
  src/pajelib/PajeColor.h
  src/pajelib/PajeComponent.h
  src/pajelib/PajeContainer.h
  src/pajelib/PajeData.h
  src/pajelib/PajeEntity.h
  src/pajelib/PajeEventDecoder.h
  src/pajelib/PajeEventDefinition.h
  src/pajelib/PajeEvent.h
  src/pajelib/PajeFileReader.h
  src/pajelib/PajeObject.h
  src/pajelib/PajeSimulator.h
  src/pajelib/PajeTraceController.h
  src/pajelib/PajeType.h)

SET(LIBPAJE_SOURCES
  src/pajelib/PajeEventDefinition.cc
  src/pajelib/PajeEvent.cc
  src/pajelib/PajeData.cc
  src/pajelib/PajeFileReader.cc
  src/pajelib/PajeEventDecoder.cc
  src/pajelib/PajeSimulator.cc
  src/pajelib/PajeSimulator+Queries.cc
  src/pajelib/PajeSimulator+Commands.cc
  src/pajelib/PajeTraceController.cc
  src/pajelib/PajeComponent.cc
  src/pajelib/PajeType.cc
  src/pajelib/PajeContainer.cc
  src/pajelib/PajeColor.cc
  src/pajelib/PajeEntity.cc)
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
  src/tupilib/tupi.h)
SET(LIBTUPI_SOURCES
  src/tupilib/bary.c
  src/tupilib/basic.c
  src/tupilib/dynar.c
  src/tupilib/dict.c
  src/tupilib/particle.c
  src/tupilib/box.c
  src/tupilib/cell.c
  src/tupilib/layout.c
  src/tupilib/node.c)
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
INCLUDE_DIRECTORIES (pj_validate ${PROJECT_SOURCE_DIR}/src/pajelib/)
TARGET_LINK_LIBRARIES(pj_validate paje)

#
# Paje Dump
#
ADD_EXECUTABLE(pj_dump src/paje_dump/main.cc)
INCLUDE_DIRECTORIES (pj_dump ${PROJECT_SOURCE_DIR}/src/pajelib/)
TARGET_LINK_LIBRARIES(pj_dump paje)

#
# tupi wxwidgets viewer (loads dot files)
#
INCLUDE ( "${wxWidgets_USE_FILE}" )
SET(TUPIWX_SRC
  src/tupiwx/main.cc
  src/tupiwx/basic.cc
  src/tupiwx/tupiew.cc
  src/tupiwx/runner.cc)
ADD_EXECUTABLE(tupiwx ${TUPIWX_SRC})
SET_TARGET_PROPERTIES(tupiwx PROPERTIES OUTPUT_NAME tupi)
INCLUDE_DIRECTORIES (tupiwx ${PROJECT_SOURCE_DIR}/src/tupilib/)
INCLUDE_DIRECTORIES(${graphviz_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(tupiwx ${wxWidgets_LIBRARIES} tupi ${graphviz_GVC_LIBRARY} ${graphviz_GRAPH_LIBRARY})

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
include_directories(viva ${PROJECT_SOURCE_DIR}/src/tupilib/)
include(${wxWidgets_USE_FILE})
target_link_libraries(viva ${wxWidgets_LIBRARIES} ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${Cairo_LIBRARIES} ${GTK2_LIBRARIES} ${LIBCONFIG_LIBRARIES} tupi paje )

#
# For pkgconfig
#
ADD_SUBDIRECTORY(pkgconfig)