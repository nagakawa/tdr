# ########## Project setup ##########
PROJECT(AGL)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

SET(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
SET(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -O3")

# ########## AGL executable ##########
# Sources:
SET(AGL_library_SRCS
    LayeredPriorityRenderable.cpp
    ShaderProgram.cpp
    AGL.rc
    Shader.cpp
    GLFWApplication.cpp
    BlendMode.cpp
    Texture.cpp
    Text.cpp
    Sprite2D.cpp
    EBO.cpp
    VBO.cpp
    Renderable.cpp
    VAO.cpp
    cairoutil.cpp
)

# Headers:
SET(AGL_library_HDRS
    resource.h
    GLFWApplication.h
    ShaderProgram.h
    Shader.h
    begincbackdecl.h
    cairoutil.h
    Texture.h
    EBO.h
    BlendMode.h
    endcbackdecl.h
    VBO.h
    LayeredPriorityRenderable.h
    VAO.h
    Text.h
    Renderable.h
    Sprite2D.h
    rect.h
)

INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("${PROJECT_INCLUDE_DIR}")

# actual target:
ADD_LIBRARY(${PROJECT_NAME} STATIC ${AGL_library_SRCS})

# add install target:
INSTALL(TARGETS AGL DESTINATION bin)
