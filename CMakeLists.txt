# ########## Project setup ##########
PROJECT(tdr)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

SET(TDR_VERSION_MAJOR "0")
SET(TDR_VERSION_MINOR "0")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(FileUtil)
add_subdirectory(AGL)
add_subdirectory(AGLTest)

FIND_PACKAGE(OpenGL)

if (OPENGL_FOUND)
  INCLUDE_DIRECTORIES("${OPENGL_INCLUDE_DIR}")
endif()

FIND_PACKAGE(GLEW)

if (GLEW_FOUND)
  INCLUDE_DIRECTORIES("${GLEW_INCLUDE_DIR}")
endif()

# actual target:
# ADD_LIBRARY(tdr ${tdr_library}_SCRS ${tdr_library_HDRS})
# SET_TARGET_PROPERTIES(tdr PROPERTIES VERSION 0.0.0)

# add install target:
# INSTALL(TARGETS tdr DESTINATION lib)
