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
enable_testing()
SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(LIB_INSTALL_DIR lib${LIB_SUFFIX} CACHE PATH "Directory where libraries are installed (relative to CMAKE_INSTALL_PREFIX).")

# the cmake options
OPTION(PAJE_LIBRARY "The Paje library with all the basic components" ON)
OPTION(PAJE_TOOLS     "Compile auxiliary tools (pj_dump, etc)" ON)
OPTION(STATIC_LINKING "Static linking of auxiliary tools" OFF)
OPTION(PAJE_DOC "The Paje Trace File documentation" OFF)

add_subdirectory(src)
if(PAJE_DOC)
  add_subdirectory(doc)
endif(PAJE_DOC)

install(PROGRAMS ${CMAKE_HOME_DIRECTORY}/scripts/pj_gantt DESTINATION bin)
