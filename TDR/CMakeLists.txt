# ########## Project setup ##########
PROJECT(TDR)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

INCLUDE(../dependencies.cmake)

SET(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/AGL ${CMAKE_SOURCE_DIR}/FileUtil)
# ########## AGL executable ##########
# Sources:
SET(TDR_library_SRCS
  Bullet.cpp
  CommonData.cpp
  Replay.cpp
  bitwise.cpp
  Game.cpp
  Player.cpp
)

# Headers:
SET(TDR_library_HDRS
    Bullet.h
    CommonData.h
    Replay.h
    defset.h
    bitwise.h
    Game.h
    Player.h
)

INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("${PROJECT_INCLUDE_DIR}")

SET(SHADER_CONTENTS_LOCATION
  ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/TDR.dir)

SET_SOURCE_FILES_PROPERTIES(
  CMakeFiles/TDR.dir/shader-contents.o
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)

ADD_CUSTOM_COMMAND(OUTPUT CMakeFiles/TDR.dir/shader-contents.o
  COMMAND ld -r -b binary -o ${SHADER_CONTENTS_LOCATION}/shader-contents.o *.glsl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
  COMMAND objcopy --rename-section .data=.rodata,alloc,load,readonly,data,contents ${SHADER_CONTENTS_LOCATION}/shader-contents.o ${SHADER_CONTENTS_LOCATION}/shader-contents.o
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl
)

# actual target:
ADD_LIBRARY(${PROJECT_NAME} STATIC
  ${TDR_library_SRCS}
  CMakeFiles/TDR.dir/shader-contents.o)

# add install target:
INSTALL(TARGETS TDR DESTINATION lib)
