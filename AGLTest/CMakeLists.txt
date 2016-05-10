# ########## Project setup ##########
PROJECT(AGLTest)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -O3")

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
# actual target:
ADD_EXECUTABLE(AGLTest ${AGLTest_executable_SRCS})
include_directories("${PROJECT_INCLUDE_DIR}")

# add install target:
INSTALL(TARGETS AGLTest DESTINATION bin)
