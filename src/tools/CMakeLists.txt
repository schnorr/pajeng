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

IF(NOT PAJE_LIBRARY)
  MESSAGE(FATAL_ERROR "Enable PAJE_LIBRARY to compile paje tools")
ENDIF(NOT PAJE_LIBRARY)

#pj_validade
ADD_EXECUTABLE(pj_validate pj_validate.cc)
INCLUDE_DIRECTORIES (pj_validate ${PROJECT_SOURCE_DIR}/src/libpaje/)
INCLUDE_DIRECTORIES(${pajeng_SOURCE_DIR}/src/fb/)
include_directories(${CMAKE_BINARY_DIR})
find_package(Boost)
if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()


IF(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pj_validate paje_library_static)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(pj_validate argp)
  ENDIF(APPLE)
ELSE(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pj_validate paje_library)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(pj_validate argp)
  ENDIF(APPLE)
ENDIF(STATIC_LINKING)
SET_PROPERTY(TARGET pj_validate PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/pj_validate DESTINATION bin)

#pj_dump
ADD_EXECUTABLE(pj_dump pj_dump.cc)
INCLUDE_DIRECTORIES (pj_dump ${PROJECT_SOURCE_DIR}/src/libpaje/)
INCLUDE_DIRECTORIES(${pajeng_SOURCE_DIR}/src/fb/)
include_directories(${CMAKE_BINARY_DIR})
find_package(Boost)
if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()

IF(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pj_dump paje_library_static)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(pj_dump argp)
  ENDIF(APPLE)
ELSE(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pj_dump paje_library)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(pj_dump argp)
  ENDIF(APPLE)
ENDIF(STATIC_LINKING)
SET_PROPERTY(TARGET pj_dump PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/pj_dump DESTINATION bin)

#pj_equals
ADD_EXECUTABLE(pj_equals pj_equals.cc)
INCLUDE_DIRECTORIES (pj_equals ${PROJECT_SOURCE_DIR}/src/libpaje/)
INCLUDE_DIRECTORIES(${pajeng_SOURCE_DIR}/src/fb/)
include_directories(${CMAKE_BINARY_DIR})
# FIXME check for libgomp / OpenMP support
set_target_properties(pj_equals PROPERTIES COMPILE_FLAGS "-fopenmp")
target_link_libraries(pj_equals gomp)
find_package(Boost)
if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()

IF(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pj_equals paje_library_static)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(pj_equals argp)
  ENDIF(APPLE)
ELSE(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pj_equals paje_library)
  IF(APPLE)
    TARGET_LINK_LIBRARIES(pj_equals argp)
  ENDIF(APPLE)
ENDIF(STATIC_LINKING)
SET_PROPERTY(TARGET pj_equals PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/pj_equals DESTINATION bin)

#
# test with tesh (thanks SimGrid team)
#
#testing the pajeng suite (with pj_dump and pj_validate) through tesh
FILE(COPY ${CMAKE_HOME_DIRECTORY}/tests/scripts/ DESTINATION ${CMAKE_BINARY_DIR}/bin)

# the macro to add tesh'es
SET(TESH_COMMAND perl -I${CMAKE_BINARY_DIR}/bin/ ${CMAKE_BINARY_DIR}/bin/tesh)
MACRO(ADD_TESH NAME)
  SET(ARGT ${ARGV})
  LIST(REMOVE_AT ARGT 0)
  ADD_TEST(${NAME} ${TESH_COMMAND} ${TESH_OPTION} ${ARGT})
ENDMACRO()

# include all tesh scripts in the tests directory
FILE(GLOB tests "${CMAKE_HOME_DIRECTORY}/tests/*.tesh")
LIST(SORT tests)
FOREACH(file ${tests})
  STRING(REGEX REPLACE "^.*/tests/" "" name ${file})
  STRING(REGEX REPLACE ".tesh$" "" testname ${name})
  ADD_TESH(${testname}  --cd "${CMAKE_BINARY_DIR}" ${file})
ENDFOREACH()
