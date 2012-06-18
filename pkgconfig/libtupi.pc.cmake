prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: tupi
Description: A barnes-hut force-directed particle system library written in C.
Version: @LIBTUPI_APIVERSION@
Libs: -L${libdir} -ltupi
Cflags: -I${includedir}