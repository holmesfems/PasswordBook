cmake_minimum_required (VERSION 3.0.2)

project (PasswordBook)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_library ( LIB_READLINE
	NAMES readline libreadline
	)

find_package ( Boost
	COMPONENTS filesystem system
	)

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

option (DEBUG "debug output" OFF)
if (DEBUG)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
endif()

# \ref http://stackoverflow.com/questions/10851247/how-to-activate-c-11-in-cmake
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	message(FATAL "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_subdirectory (src)
add_subdirectory (doc)

