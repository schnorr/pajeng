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
# The Paje Library
#
SET(PAJE_LIB_SRC
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
  src/pajelib/PajeEntity.cc
)
ADD_LIBRARY(pajelib SHARED ${PAJE_LIB_SRC})

#
# The Tupi Library written in C (particle system)
#
SET(TUPI_LIB_SRC
  src/tupilib/bary.c
  src/tupilib/basic.c
  src/tupilib/dynar.c
  src/tupilib/dict.c
  src/tupilib/particle.c
  src/tupilib/box.c
  src/tupilib/cell.c
  src/tupilib/layout.c
  src/tupilib/node.c)
ADD_LIBRARY(tupilib SHARED ${TUPI_LIB_SRC})
TARGET_LINK_LIBRARIES(tupilib m)

#
# Paje Validate (checks if a trace file is valid or not)
# 
ADD_EXECUTABLE(paje_validate src/paje_validate/main.cc)
INCLUDE_DIRECTORIES (paje_validate ${PROJECT_SOURCE_DIR}/src/pajelib/)
TARGET_LINK_LIBRARIES(paje_validate pajelib)

#
# tupi wxwidgets viewer (loads dot files)
#
INCLUDE ( "${wxWidgets_USE_FILE}" )
SET(TUPIWX_SRC
  src/tupiwx/main.cc
  src/tupiwx/basic.cc
  src/tupiwx/tupiew.cc
  src/tupiwx/runner.cc)
ADD_EXECUTABLE(tupi ${TUPIWX_SRC})
INCLUDE_DIRECTORIES (paje_validate ${PROJECT_SOURCE_DIR}/src/tupilib/)
INCLUDE_DIRECTORIES(${graphviz_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(tupi ${wxWidgets_LIBRARIES} tupilib ${graphviz_GVC_LIBRARY} ${graphviz_GRAPH_LIBRARY})

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
target_link_libraries(viva ${wxWidgets_LIBRARIES} ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${Cairo_LIBRARIES} ${GTK2_LIBRARIES} ${LIBCONFIG_LIBRARIES} tupilib pajelib )

