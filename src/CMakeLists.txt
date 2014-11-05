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

# The Paje Library (libpaje) 
IF(PAJE_LIBRARY)
  add_subdirectory (${CMAKE_HOME_DIRECTORY}/src/libpaje/)
ENDIF(PAJE_LIBRARY)

# Paje tools
IF(PAJE_TOOLS)
  add_subdirectory (${CMAKE_HOME_DIRECTORY}/src/tools)
ENDIF(PAJE_TOOLS)

# The Paje Utils Library (libpajeutils)
IF(PAJE_UTILS_LIBRARY)
  add_subdirectory (${CMAKE_HOME_DIRECTORY}/src/utils)
ENDIF(PAJE_UTILS_LIBRARY)

# The PajeNG
IF(PAJENG)
  add_subdirectory (${CMAKE_HOME_DIRECTORY}/src/pajeng)
ENDIF(PAJENG)
