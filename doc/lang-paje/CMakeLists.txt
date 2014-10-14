cmake_minimum_required(VERSION 2.8)

include(../../cmake/UseLATEX.cmake)

add_latex_document(lang-paje.tex DEFAULT_PDF INPUTS definitions.tex)