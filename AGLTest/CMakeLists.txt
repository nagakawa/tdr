# ########## Project setup ##########
PROJECT(AGLTest)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

INCLUDE(../dependencies.cmake)

SET(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../AGL ${PROJECT_SOURCE_DIR}/../FileUtil)

# ########## AGLTest executable ##########
# Sources:
SET(AGLTest_executable_SRCS
    Test.cpp
)

# Headers:
SET(AGLTest_executable_HDRS
    Test.h
)


include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")
# actual target:
ADD_EXECUTABLE(AGLTest ${AGLTest_executable_SRCS})

TARGET_LINK_LIBRARIES(AGLTest ${CMAKE_SOURCE_DIR}/build/libAGL.a ${CMAKE_SOURCE_DIR}/build/libFileUtil.a ${PangoCairo_LIBRARIES} ${Pango_1_0_LIBRARY} ${CAIRO_LIBRARIES} ${GLFW_LIBRARY_RELEASE} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SOIL_LIBRARY} ${GOBJECT_LIBRARIES} ${GLIB_LIBRARIES} -lm)
ADD_CUSTOM_COMMAND(
  TARGET AGLTest
  POST_BUILD
  COMMAND cp -r shader textures ${CMAKE_SOURCE_DIR}/build
)

# add install target:
INSTALL(TARGETS AGLTest DESTINATION bin)
