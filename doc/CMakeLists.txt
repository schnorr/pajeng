find_package(ASCIIDoc REQUIRED)

set(MAN_OUTPUTS "")

foreach(_INPUT pj_equals.1.txt pj_dump.1.txt pj_gantt.1.txt)
  # Remove only the .txt suffix → pj_equals.1
  string(REGEX REPLACE "\\.txt$" "" _OUTPUT "${_INPUT}")

  set(_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${_INPUT})
  set(_DST ${CMAKE_CURRENT_BINARY_DIR}/${_OUTPUT})

  add_custom_command(
    OUTPUT ${_DST}
    COMMAND ${A2X_EXECUTABLE} --xsltproc-opts=--nonet
                              --doctype manpage
                              --format manpage
                              ${_SRC}
                              -D ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${_SRC}
    COMMENT "Generating man page ${_OUTPUT}"
    VERBATIM
  )

  list(APPEND MAN_OUTPUTS ${_DST})

  install(FILES ${_DST} DESTINATION ${CMAKE_INSTALL_PREFIX}/man/man1)
endforeach()

add_custom_target(man ALL DEPENDS ${MAN_OUTPUTS})

add_subdirectory (${CMAKE_HOME_DIRECTORY}/doc/lang-paje/)
