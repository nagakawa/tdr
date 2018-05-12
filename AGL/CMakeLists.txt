# ########## Project setup ##########
CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
CMAKE_POLICY(SET CMP0048 NEW)
CMAKE_POLICY(SET CMP0069 NEW)
PROJECT(AGL VERSION 0.1)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

INCLUDE(../dependencies.cmake)

# ########## AGL executable ##########
# Sources:
SET(AGL_library_SRCS
    LayeredPriorityRenderable.cpp
    ShaderProgram.cpp
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
    FBO.cpp
    Scene.cpp
    ScenedGLFWApplication.cpp
    Dimensional.cpp
    sound/Sound.cpp
    sound/Mixer.cpp
    sound/Reader.cpp
    text/GlyphDrawList.cpp
    text/Font.cpp
    text/XText.cpp
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
    FBO.h
    debug.h
    Dimensional.h
    Scene.h
    ScenedGLFWApplication.h
    sound/Sound.h
    sound/Mixer.h
    sound/Reader.h
    text/GlyphDrawList.h
    text/Font.h
    text/XText.h
)

INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("${PROJECT_INCLUDE_DIR}")

SET(SHADER_CONTENTS_LOCATION
${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/AGL.dir)

SET_SOURCE_FILES_PROPERTIES(
CMakeFiles/AGL.dir/shader-contents.o
PROPERTIES
EXTERNAL_OBJECT true
GENERATED true
)

ADD_CUSTOM_COMMAND(OUTPUT CMakeFiles/AGL.dir/shader-contents.o
    COMMAND ld -r -b binary -o ${SHADER_CONTENTS_LOCATION}/shader-contents.o *.glsl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    COMMAND objcopy --rename-section .data=.rodata,alloc,load,readonly,data,contents ${SHADER_CONTENTS_LOCATION}/shader-contents.o ${SHADER_CONTENTS_LOCATION}/shader-contents.o
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl
)

# actual target:
ADD_LIBRARY(${PROJECT_NAME} STATIC
    ${AGL_library_SRCS}
    CMakeFiles/AGL.dir/shader-contents.o)

# add install target:
INSTALL(TARGETS AGL DESTINATION lib)
