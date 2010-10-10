# Set the minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Set the project name
PROJECT(allocator)

SET(CMAKE_SOURCE_DIR ./../library/)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

SET(PROJECT_DIRECTORY "./")

# Add definitions, compiler switches, etc.
ADD_DEFINITIONS("-Wall -W -Wextra -Werror -Wformat -Wformat-y2k -Winit-self -Wstack-protector -Wunknown-pragmas -Wundef -Wcast-qual -Wwrite-strings -Wno-unused-parameter -Wpointer-arith -Wno-switch -Woverloaded-virtual -Wmissing-include-dirs -Wuninitialized -Wc++0x-compat -std=c++0x -ffor-scope -O2")

ADD_DEFINITIONS("-DUNICODE -D_UNICODE")

IF("${CMAKE_SYSTEM}" MATCHES "Linux")
  ADD_DEFINITIONS("-D__LINUX__")
ENDIF()
IF(WIN32)
  ADD_DEFINITIONS("-DNOMINMAX")
ENDIF()

#list all source files here
ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)

#need to link to some other libraries ? just add them here
TARGET_LINK_LIBRARIES(${PROJECT_NAME})

