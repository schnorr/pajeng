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

IF(NOT PAJE_FLEXBISON)
  MESSAGE(FATAL_ERROR "Enable PAJE_FLEXBISON to compile paje tools")
ENDIF(NOT PAJE_FLEXBISON)


#pj_validade
ADD_EXECUTABLE(pj_validate pj_validate.cc)
INCLUDE_DIRECTORIES (pj_validate ${PROJECT_SOURCE_DIR}/src/libpaje/)
INCLUDE_DIRECTORIES(${pajeng_SOURCE_DIR}/src/fb/)
IF(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pj_validate paje_library_static)
ELSE(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pj_validate paje_library)
ENDIF(STATIC_LINKING)
SET_PROPERTY(TARGET pj_validate PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/pj_validate DESTINATION bin)

#pj_dump
ADD_EXECUTABLE(pj_dump pj_dump.cc)
INCLUDE_DIRECTORIES (pj_dump ${PROJECT_SOURCE_DIR}/src/libpaje/)
INCLUDE_DIRECTORIES(${pajeng_SOURCE_DIR}/src/fb/)
IF(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pj_dump paje_library_static)
ELSE(STATIC_LINKING)
  TARGET_LINK_LIBRARIES(pj_dump paje_library)
ENDIF(STATIC_LINKING)
SET_PROPERTY(TARGET pj_dump PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/pj_dump DESTINATION bin)
