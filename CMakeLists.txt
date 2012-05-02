CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT(paje-cpp)

SET(CMAKE_CXX_FLAGS "-g -rdynamic")

ADD_LIBRARY(pajecpp SHARED
  src/paje/PajeEventDefinition.cc
  src/paje/PajeEvent.cc
  src/paje/PajeData.cc
  src/paje/PajeFileReader.cc
  src/paje/PajeEventDecoder.cc
  src/paje/PajeSimulator.cc
  src/paje/PajeTraceController.cc
  src/paje/PajeComponent.cc
  src/paje/PajeType.cc
)

ADD_EXECUTABLE(paje src/main-paje.cc)
INCLUDE_DIRECTORIES (src/paje/)
TARGET_LINK_LIBRARIES(paje pajecpp)