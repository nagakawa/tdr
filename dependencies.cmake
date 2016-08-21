
FIND_PACKAGE(OpenGL)

if (OPENGL_FOUND)
  INCLUDE_DIRECTORIES("${OPENGL_INCLUDE_DIR}")
else()
  MESSAGE(FATAL_ERROR "Could not find OpenGL")
endif()

FIND_PACKAGE(GLEW)

if (GLEW_FOUND)
  INCLUDE_DIRECTORIES("${GLEW_INCLUDE_DIR}")
else()
  MESSAGE(FATAL_ERROR "Could not find GLEW")
endif()

#FIND_PACKAGE(GTK2)

#if (GTK2_FOUND)
#  INCLUDE_DIRECTORIES("${GTK2_INCLUDE_DIR}")
#else()
#  MESSAGE(FATAL_ERROR "Could not find GTK2")
#endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules/")
FIND_PACKAGE(Pango REQUIRED)
INCLUDE_DIRECTORIES(${Pango_1_0_INCLUDE_DIR})

FIND_PACKAGE(Cairo REQUIRED)
INCLUDE_DIRECTORIES(${CAIRO_INCLUDE_DIRS})

FIND_PACKAGE(PangoCairo REQUIRED)
INCLUDE_DIRECTORIES(${PangoCairo_INCLUDE_DIRS})

FIND_PACKAGE(GLM REQUIRED)
INCLUDE_DIRECTORIES(${GLM_INCLUDE_DIRS})

FIND_PACKAGE(GLFW REQUIRED)
INCLUDE_DIRECTORIES(${GLFW_INCLUDE_DIRS})

FIND_PACKAGE(SOIL REQUIRED)
INCLUDE_DIRECTORIES(${SOIL_INCLUDE_DIR})

FIND_PACKAGE(GLib REQUIRED)
INCLUDE_DIRECTORIES(${GLIB_INCLUDE_DIRS})

FIND_PACKAGE(GObject REQUIRED)
INCLUDE_DIRECTORIES(${GOBJECt_INCLUDE_DIR})

SET(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../FileUtil)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror -O3 -g")
