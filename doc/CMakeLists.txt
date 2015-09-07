add_custom_target(man ALL)
find_package(ASCIIDoc REQUIRED)

foreach (_INPUT pj_validate.1.txt pj_dump.1.txt)
  string(REPLACE "." ";" LIST ${_INPUT})
  list(REMOVE_ITEM LIST "txt")
  list(GET LIST 0 1 VAR)
  string(REGEX REPLACE ";" "." _OUTPUT "${VAR}")

  add_custom_command(
    TARGET man
    SOURCE  ${CMAKE_CURRENT_SOURCE_DIR}/${_INPUT}
    COMMAND ${A2X_EXECUTABLE} --xsltproc-opts=--nonet --doctype manpage --format manpage ${CMAKE_CURRENT_SOURCE_DIR}/${_INPUT} -D ${CMAKE_CURRENT_BINARY_DIR}
    OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/${_OUTPUT}
    )

  add_custom_command(
    TARGET man
    SOURCE man
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_OUTPUT}
    )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_OUTPUT} DESTINATION ${CMAKE_INSTALL_PREFIX}/man/man1)

endforeach(_INPUT)

add_subdirectory (${CMAKE_HOME_DIRECTORY}/doc/lang-paje/)