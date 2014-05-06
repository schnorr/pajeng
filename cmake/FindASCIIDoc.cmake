# License
# This file is part of PajeNG.
#
# PajeNG is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 3 of the License, or (at your
# option) any later version.
#
# PajeNG is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with PajeNG; if not, write to the Free Software Foundation,
# Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA


# - Find the AsciiDoc text converterter
# This module locates the asciidoc executable which converts ASCII files written
# in a simple text markup into various output formats.
#
# The module sets the following cache variables
# - ASCIIDOC_EXECUTABLE the asciidoc executable
# - A2X_EXECUTABLE the a2x executable (only on UNIX systems)
#

# Copyright 2009 Michael Wild <themiwi@users.sourceforge.net>
# Copyright 2014 Lucas M. Schnorr <schnorr@inf.ufrgs.br>

# search the asciidoc executable
find_program( ASCIIDOC_EXECUTABLE NAMES asciidoc asciidoc.py )
set( __AsciiDoc_VARS ASCIIDOC_EXECUTABLE )
if( UNIX )
find_program( A2X_EXECUTABLE NAMES a2x a2x.sh )
list( APPEND __AsciiDoc_VARS A2X_EXECUTABLE )
endif( UNIX )

mark_as_advanced( ${__AsciiDoc_VARS} )

# handle standard arguments
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( AsciiDoc DEFAULT_MSG ${__AsciiDoc_VARS} )
