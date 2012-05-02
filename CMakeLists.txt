CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT(paje-cpp)

SET(CMAKE_CXX_FLAGS "-g -rdynamic")

ADD_LIBRARY(pajecpp SHARED
  src/PajeEventDefinition.cc
  src/PajeEvent.cc
  src/PajeData.cc
  src/PajeFileReader.cc
  src/PajeEventDecoder.cc
  src/PajeSimulator.cc
  src/PajeTraceController.cc
  src/PajeComponent.cc
  src/PajeType.cc
)

SET(SRC
  src/main.cc
)

ADD_EXECUTABLE(paje ${SRC})
TARGET_LINK_LIBRARIES(paje pajecpp)