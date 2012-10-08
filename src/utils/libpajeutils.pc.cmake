prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: pajeutils
Description: An auxiliary library for libPaje with QT-dependent classes
Version: @LIBPAJEUTILS_APIVERSION@
Libs: -L${libdir} -lpajeutils
Cflags: -I${includedir}
