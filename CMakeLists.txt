CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT(paje-cpp)

SET(CMAKE_CXX_FLAGS "-g -rdynamic")

SET(SRC
  src/main.cc
  src/PajeFileReader.cc
  src/PajeEventDecoder.cc
  src/PajeSimulator.cc
  src/PajeTraceController.cc
  src/PajeComponent.cc
)

ADD_EXECUTABLE(paje ${SRC})