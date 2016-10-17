# ########## Project setup ##########
PROJECT(TDR)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

INCLUDE(../dependencies.cmake)

SET(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../AGL ${PROJECT_SOURCE_DIR}/../FileUtil)

# ########## AGL executable ##########
# Sources:
SET(TDR_library_SRCS
  Bullet.cpp
  CommonData.cpp
  Replay.cpp
  Collidable.cpp
  bitwise.cpp
  fixedpoint.cpp
  hitbox.cpp
  CollisionRegistrar.cpp
  CollisionIterator.cpp
  Game.cpp
)

# Headers:
SET(TDR_library_HDRS
    Bullet.h
    CommonData.h
    Replay.h
    defset.h
    Collidable.h
    bitwise.h
    fixedpoint.h
    hitbox.h
    CollisionIterator.h
    CollisionRegistrar.h
    Game.h
)

INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("${PROJECT_INCLUDE_DIR}")

# actual target:
ADD_LIBRARY(${PROJECT_NAME} STATIC ${TDR_library_SRCS})

# add install target:
INSTALL(TARGETS TDR DESTINATION lib)
