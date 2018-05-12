# ########## Project setup ##########
CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
CMAKE_POLICY(SET CMP0048 NEW)
CMAKE_POLICY(SET CMP0069 NEW)
PROJECT(TDRTest VERSION 0.1)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

INCLUDE(../dependencies.cmake)

SET(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../AGL ${PROJECT_SOURCE_DIR}/../FileUtil ${PROJECT_SOURCE_DIR}/../TDR)

# ########## AGL executable ##########
# Sources:
SET(TDRTest_executable_SRCS
  Test.cpp
)

# Headers:
SET(TDRTest_executable_HDRS
    Test.h
)

INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("${PROJECT_INCLUDE_DIR}")

ADD_EXECUTABLE(TDRTest ${TDRTest_executable_SRCS})

TARGET_LINK_LIBRARIES(TDRTest
  ${CMAKE_SOURCE_DIR}/build/libTDR.a
  ${CMAKE_SOURCE_DIR}/build/libAGL.a
  ${CMAKE_SOURCE_DIR}/build/libFileUtil.a
  ${CMAKE_SOURCE_DIR}/3rdparty/agc/kozet_coroutine/build/libkozet_coroutine.a
  ${PangoCairo_LIBRARIES}
  ${Pango_1_0_LIBRARY}
  ${CAIRO_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${SOIL_LIBRARY}
  ${GOBJECT_LIBRARIES}
  ${GLIB_LIBRARIES}
  ${PORTAUDIO_LIBRARIES}
  ${VORBIS_LIBRARIES}
  -lm)
ADD_CUSTOM_COMMAND(
  TARGET TDRTest
  POST_BUILD
  COMMAND cp -r textures-tdr ${CMAKE_SOURCE_DIR}/build
)

# add install target:
INSTALL(TARGETS TDRTest DESTINATION lib)
