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
# The following line should be deleted in some Ubuntu releases
cmake_policy(SET CMP0050 OLD)
enable_testing()


SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(LIB_INSTALL_DIR lib${LIB_SUFFIX} CACHE PATH "Directory where libraries are installed (relative to CMAKE_INSTALL_PREFIX).")

# the cmake options
OPTION(PAJENG         "The PajeNG visualization tool (Qt4 version)" ON)
OPTION(PAJE_UTILS_LIBRARY "The Paje Utils library with QT-dependent components" ON)
OPTION(PAJE_LIBRARY "The Paje library with all the basic components" ON)
OPTION(PAJE_TOOLS     "Compile auxiliary tools (pj_dump, etc)" ON)
OPTION(STATIC_LINKING "Static linking of auxiliary tools" OFF)
OPTION(PAJE_DOC "The Paje Trace File documentation" OFF)

add_subdirectory(src)
if(PAJE_DOC)
  add_subdirectory(doc)
endif(PAJE_DOC)
find_package(Boost)
if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()

