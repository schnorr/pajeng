# pkgconfig CmakeLists.txt

CONFIGURE_FILE (libpaje.pc.cmake libpaje.pc @ONLY)
INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libpaje.pc DESTINATION lib/pkgconfig)