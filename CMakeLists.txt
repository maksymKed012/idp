CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_CXX_COMPILER "g++")
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "-Wall")

#====================================================================

PROJECT(IDP)

FILE(
  GLOB 
  idp_src
  *.hpp
  *.cpp
)

ADD_EXECUTABLE(idp_test ${idp_src})
