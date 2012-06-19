CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT(viva)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Define sources, headers for all targets
#
# The Paje Library (libpaje)
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
#
# The Tupi Library written in C (particle system)
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
#
# Paje Validate (checks if a trace file is valid or not)
SET(PAJEVALIDATE_SOURCES src/paje_validate/main.cc)
#
# Paje Dump
SET(PAJEDUMP_SOURCES src/paje_dump/main.cc)
#
# Viva Sources
SET(VIVA_SOURCES
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

#
# the cmake options
#
OPTION(PAJE_LIBRARY "The Paje library with all the basic components" ON)
OPTION(PAJE_DUMP    "Compile pj_dump (program to dump the contents of a trace" ON)
OPTION(PAJE_VALIDATE "Compile pj_validate (program to validate a trace" ON)
OPTION(TUPI_LIBRARY "The interactive Barnes-Hut particle system library" OFF)
OPTION(VIVA         "Compile viva (a hierarchical graph visualization tool" OFF)

#
# The Paje Library (libpaje)
# 
IF(PAJE_LIBRARY)
  FIND_PACKAGE (Boost REQUIRED)
  ADD_LIBRARY(paje_library SHARED ${LIBPAJE_SOURCES})
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
ENDIF(PAJE_LIBRARY)

#
# The Tupi Library written in C (particle system)
# 
IF(TUPI_LIBRARY)
  ADD_LIBRARY(tupi_library SHARED ${LIBTUPI_SOURCES})
  SET_TARGET_PROPERTIES(tupi_library PROPERTIES OUTPUT_NAME tupi)
  TARGET_LINK_LIBRARIES(tupi_library m)
  SET_TARGET_PROPERTIES(tupi_library PROPERTIES
    # bump when new function are added in the library
    VERSION 1.0
    # bump when ABI is broken (symbols removed or parameters changed)
    SOVERSION 1)
  # bump when the API change (new functions in the header file)
  SET(LIBTUPI_APIVERSION 1)
  INSTALL(TARGETS tupi_library DESTINATION lib)
  INSTALL(FILES ${LIBTUPI_HEADERS} DESTINATION include)
ENDIF(TUPI_LIBRARY)

#
# Paje Validate (checks if a trace file is valid or not)
#
IF(PAJE_VALIDATE)
  IF(NOT PAJE_LIBRARY)
    MESSAGE(FATAL_ERROR "You must enable PAJE_LIBRARY to compile pj_validate")
  ENDIF(NOT PAJE_LIBRARY)
  ADD_EXECUTABLE(pj_validate ${PAJEVALIDATE_SOURCES})
  INCLUDE_DIRECTORIES (pj_validate ${PROJECT_SOURCE_DIR}/src/libpaje/)
  TARGET_LINK_LIBRARIES(pj_validate paje_library)
  INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pj_validate DESTINATION bin)
ENDIF(PAJE_VALIDATE)

#
# Paje Dump
#
IF(PAJE_DUMP)
  IF(NOT PAJE_LIBRARY)
    MESSAGE(FATAL_ERROR "You must enable PAJE_LIBRARY to compile pj_dump")
  ENDIF(NOT PAJE_LIBRARY)
  ADD_EXECUTABLE(pj_dump ${PAJEDUMP_SOURCES})
  INCLUDE_DIRECTORIES (pj_dump ${PROJECT_SOURCE_DIR}/src/libpaje/)
  TARGET_LINK_LIBRARIES(pj_dump paje_library)
  INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pj_dump DESTINATION bin)
ENDIF(PAJE_DUMP)

#
# Viva Sources
#
IF(VIVA)
  IF(NOT PAJE_LIBRARY)
    MESSAGE(FATAL_ERROR "You must enable PAJE_LIBRARY to compile viva")
  ENDIF(NOT PAJE_LIBRARY)
  IF(NOT TUPI_LIBRARY)
    MESSAGE(FATAL_ERROR "You must enable TUPI_LIBRARY to compile viva")
  ENDIF(NOT TUPI_LIBRARY)

  FIND_PACKAGE(wxWidgets COMPONENTS gl core base REQUIRED)
  FIND_PACKAGE(OpenGL REQUIRED)
  FIND_PACKAGE(LibConfig REQUIRED)
  FIND_PACKAGE(GTK2 REQUIRED)
  FIND_PACKAGE(GLUT REQUIRED)

  ADD_EXECUTABLE(viva ${VIVA_SOURCES})
  include_directories(viva ${GTK2_INCLUDE_DIRS})
  include_directories(viva ${Cairo_INCLUDE_DIRS})
  include_directories(viva ${PROJECT_SOURCE_DIR}/src/libtupi/)
  include_directories(viva ${PROJECT_SOURCE_DIR}/src/libpaje/)
  include(${wxWidgets_USE_FILE})
  target_link_libraries(viva ${wxWidgets_LIBRARIES} ${GLUT_glut_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${Cairo_LIBRARIES} ${GTK2_LIBRARIES} ${LIBCONFIG_LIBRARIES} tupi_library paje_library )
  INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/viva DESTINATION bin)
ENDIF(VIVA)

#
# For pkgconfig
#
ADD_SUBDIRECTORY(pkgconfig)