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

FIND_PACKAGE(FLEX REQUIRED)
FIND_PACKAGE(BISON REQUIRED)
FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc COMPILE_FLAGS "-F")
BISON_TARGET(parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc VERBOSE parser.output)

# The Paje Library (libpaje)
SET(LIBPAJE_HEADERS
  PajeColor.h
  PajeException.h
  PajeProtocols.h
  PajeComponent.h
  PajeContainer.h
  PajeData.h
  PajeEntity.h
  PajeEventDecoder.h
  PajeEventDefinition.h
  PajeTraceEvent.h
  PajeEvent.h
  PajeFileReader.h
  PajeObject.h
  PajeSimulator.h
  PajeProbabilisticSimulator.h
  PajeType.h
  PajeValue.h
  PajeDefinitions.h
  PajeFlexReader.h
  PajeUnity.h
  PajeEnum.h
)
SET(LIBPAJE_SOURCES
  PajeEventDefinition.cc
  PajeTraceEvent.cc
  PajeEvent.cc
  PajeData.cc
  PajeFileReader.cc
  PajeObject.cc
  PajeEventDecoder.cc
  PajeSimulator.cc
  PajeProbabilisticSimulator.cc
  PajeSimulator+Queries.cc
  PajeSimulator+Commands.cc
  PajeComponent.cc
  PajeType.cc
  PajeValue.cc
  PajeEntity.cc
  PajeContainer.cc
  PajeColor.cc
  PajeException.cc
  PajeDefinitions.cc
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
  PajeFlexReader.cc
  PajeUnity.cc
)

FIND_PACKAGE(Boost REQUIRED)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_HOME_DIRECTORY}/src/libpaje)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

IF(STATIC_LINKING)
  # static library
  ADD_LIBRARY(paje_library_static STATIC ${LIBPAJE_SOURCES})
  TARGET_LINK_LIBRARIES(paje_library_static ${FLEX_LIBRARIES})
  SET_TARGET_PROPERTIES(paje_library_static PROPERTIES OUTPUT_NAME paje)
ELSE(STATIC_LINKING)
  # shared library
  ADD_LIBRARY(paje_library SHARED ${LIBPAJE_SOURCES})
  TARGET_LINK_LIBRARIES(paje_library ${FLEX_LIBRARIES})
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
  CONFIGURE_FILE (libpaje.pc.cmake libpaje.pc @ONLY)
  INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libpaje.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
ENDIF(STATIC_LINKING)

#### VERSION CHECK

###############
## GIT version check
##
if(EXISTS ${CMAKE_HOME_DIRECTORY}/.git/)
  execute_process(COMMAND git remote
  COMMAND head -n 1
  WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/.git/
  OUTPUT_VARIABLE remote
  RESULT_VARIABLE ret
  )
  string(REPLACE "\n" "" remote "${remote}")
  execute_process(COMMAND git config --get remote.${remote}.url
  WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/.git/
  OUTPUT_VARIABLE url
  RESULT_VARIABLE ret
  )
  string(REPLACE "\n" "" url "${url}")
  #message(STATUS "Git url: ${url}")
  if(url)
    execute_process(COMMAND git --git-dir=${CMAKE_HOME_DIRECTORY}/.git log --pretty=oneline --abbrev-commit -1
    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/.git/
    OUTPUT_VARIABLE GIT_VERSION
    RESULT_VARIABLE ret
    )
    string(REPLACE "\n" "" GIT_VERSION "${GIT_VERSION}")
    message(STATUS "Git version: ${GIT_VERSION}")
    execute_process(COMMAND git --git-dir=${CMAKE_HOME_DIRECTORY}/.git log -n 1 --pretty=format:%ai .
    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/.git/
    OUTPUT_VARIABLE GIT_DATE
    RESULT_VARIABLE ret
    )
    string(REPLACE "\n" "" GIT_DATE "${GIT_DATE}")
    message(STATUS "Git date: ${GIT_DATE}")
    string(REGEX REPLACE " .*" "" GIT_VERSION "${GIT_VERSION}")

    execute_process(COMMAND git --git-dir=${CMAKE_HOME_DIRECTORY}/.git log --pretty=format:%H -1
                    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/.git/
                    OUTPUT_VARIABLE LIBPAJE_GITHASH
                    RESULT_VARIABLE ret
                    )
    string(REPLACE "\n" "" LIBPAJE_GITHASH "${LIBPAJE_GITHASH}")

  endif()
elseif(EXISTS ${CMAKE_HOME_DIRECTORY}/.gitversion)
  FILE(STRINGS ${CMAKE_HOME_DIRECTORY}/.gitversion GIT_VERSION)
endif()

if(release)
  set(LIBPAJE_VERSION_STRING "${LIBPAJE_VERSION_STRING}\\nRelease build")
else()
  set(LIBPAJE_VERSION_STRING "${LIBPAJE_VERSION_STRING}\\nDevelopment build")
endif()
if(GIT_VERSION)
  set(LIBPAJE_VERSION_STRING "${LIBPAJE_VERSION_STRING} at commit ${GIT_VERSION}")
endif()
if(GIT_DATE)
  set(LIBPAJE_VERSION_STRING "${LIBPAJE_VERSION_STRING} (${GIT_DATE})")
endif()

configure_file("${CMAKE_HOME_DIRECTORY}/src/libpaje/libpaje_config.h.in" "${CMAKE_BINARY_DIR}/libpaje_config.h" @ONLY IMMEDIATE)
