cmake_minimum_required(VERSION 3.20...4.0)
include(../../cmake/UseLATEX.cmake)
add_latex_document(lang-paje.tex DEFAULT_PDF INPUTS definitions.tex)
