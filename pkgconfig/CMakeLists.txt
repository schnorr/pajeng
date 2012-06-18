# pkgconfig CmakeLists.txt

CONFIGURE_FILE (libpaje.pc.cmake libpaje.pc @ONLY)
INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libpaje.pc DESTINATION lib/pkgconfig)

CONFIGURE_FILE (libtupi.pc.cmake libtupi.pc @ONLY)
INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/libtupi.pc DESTINATION lib/pkgconfig)
