cmake_minimum_required (VERSION 3.0.2)

project (PasswordBook)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

## Check c++11 compatibility
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

## Check dependency
find_package(PkgConfig)
find_library ( LIB_READLINE
	NAMES readline libreadline
	)
find_package ( Boost
	COMPONENTS filesystem system
	)
pkg_check_modules (BOTAN REQUIRED botan-2>=2.0.1)

## Set Compilation Database for YouCompleteMe
# \ref http://bastian.rieck.ru/blog/posts/2015/ycm_cmake/
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message ( "${CMAKE_CURRENT_BINARY_DIR}" )
if( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
	execute_process( COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
		${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
		)
endif()

option (DEBUG "debug output" OFF)
if (DEBUG)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DDEBUG")
endif()

add_subdirectory (src)

option (enable_test "enable tests" ON)
if (enable_test)
	enable_testing()
	find_package(GTest REQUIRED)
	add_subdirectory (test)
endif()

add_subdirectory (doc)



