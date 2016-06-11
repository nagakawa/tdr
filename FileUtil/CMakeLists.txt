# ########## Project setup ##########
PROJECT(FileUtil)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

SET(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# ########## FileUtil library ##########
# Sources:
SET(FileUtil_library_SRCS
    cross.cpp
    utf8.cpp
)

# Headers:
SET(FileUtil_library_HDRS
    utf8.h
    cross.h
)

INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("${PROJECT_INCLUDE_DIR}")

# actual target:
ADD_LIBRARY(FileUtil ${FileUtil_library_SRCS} ${FileUtil_library_HDRS})
SET_TARGET_PROPERTIES(FileUtil PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS FileUtil DESTINATION lib)
