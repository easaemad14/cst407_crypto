cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
	message(FATAL_ERROR "Please make a build directory to make from!")
endif()

set(CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall -Wextra -Werror -Wpedantic")

# Create variables to list sources and includes files, respectively
set(SRCS main.cpp)

# Add our executable to be built from our files
add_executable(sdes ${SRCS})
