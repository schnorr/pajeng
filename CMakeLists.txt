CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT(viva)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
FIND_PACKAGE(wxWidgets COMPONENTS gl core base REQUIRED)
FIND_PACKAGE(LibConfig REQUIRED)
FIND_PACKAGE(GTK2 REQUIRED)
FIND_PACKAGE(Cairo)

SET(CMAKE_CXX_FLAGS "-g -rdynamic")

SET(PAJE_LIB_SRC
  src/paje/PajeEventDefinition.cc
  src/paje/PajeEvent.cc
  src/paje/PajeData.cc
  src/paje/PajeFileReader.cc
  src/paje/PajeEventDecoder.cc
  src/paje/PajeSimulator.cc
  src/paje/PajeSimulator+Queries.cc
  src/paje/PajeSimulator+Commands.cc
  src/paje/PajeTraceController.cc
  src/paje/PajeComponent.cc
  src/paje/PajeType.cc
  src/paje/PajeContainer.cc
  src/paje/PajeColor.cc
  src/paje/PajeEntity.cc
)
ADD_LIBRARY(pajecpp SHARED ${PAJE_LIB_SRC})

SET(TUPI_LIB_SRC
  src/tupi/bary.c
  src/tupi/basic.c
  src/tupi/dynar.c
  src/tupi/dict.c
  src/tupi/particle.c
  src/tupi/box.c
  src/tupi/cell.c
  src/tupi/layout.c
  src/tupi/node.c)
ADD_LIBRARY(tupic SHARED ${TUPI_LIB_SRC})
TARGET_LINK_LIBRARIES(tupic m)

INCLUDE_DIRECTORIES (src/paje/)
INCLUDE_DIRECTORIES (src/tupi/)
INCLUDE( "${wxWidgets_USE_FILE}" )

# paje command-line executable
ADD_EXECUTABLE(paje src/main-paje.cc)
TARGET_LINK_LIBRARIES(paje pajecpp)

# tupi wxwidgets viewer (loads dot files)
SET(TUPIWX_SRC
  src/tupiwx/main.cc
  src/tupiwx/basic.cc
  src/tupiwx/tupiew.cc
  src/tupiwx/runner.cc)
ADD_EXECUTABLE(tupi ${TUPIWX_SRC})
TARGET_LINK_LIBRARIES(tupi ${wxWidgets_LIBRARIES} tupic gvc graph)

# viva executable
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
target_link_libraries(viva ${wxWidgets_LIBRARIES} ${Cairo_LIBRARIES} ${GTK2_LIBRARIES} ${LIBCONFIG_LIBRARIES} tupic pajecpp )

