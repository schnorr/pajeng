prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: paje
Description: A library to read, simulate and filter paje traces prior to visualization
Version: @LIBPAJE_APIVERSION@
Libs: -L${libdir} -lpaje
Cflags: -I${includedir}
