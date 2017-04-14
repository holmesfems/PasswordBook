cmake_minimum_required (VERSION 3.0.2)

project (PasswordBook VERSION 0.0.1)

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

option (BUILD_WEB_SERVICE "also build web service" ON)

## Check dependency
# TODO https://cmake.org/Wiki/CMake:How_To_Find_Libraries
find_package(PkgConfig)
find_library ( LIB_READLINE
	NAMES readline libreadline
	)
find_package ( Boost
	COMPONENTS filesystem system
	)

option (USE_SYSTEM_BOTAN "use system installed botan" ON)
if (USE_SYSTEM_BOTAN)
	pkg_check_modules (BOTAN REQUIRED botan-2>=2.0.1)
else ()
	# TODO check python
	include (ExternalProject)
	set(BOTAN_INSTALL_DIR "${PROJECT_BINARY_DIR}/third_party")
	ExternalProject_Add (botan
		PREFIX ${BOTAN_INSTALL_DIR}
		GIT_REPOSITORY "https://github.com/randombit/botan.git"
		GIT_TAG "2.0.1"
		UPDATE_COMMAND ""
		SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/botan"
		CONFIGURE_COMMAND ${PROJECT_SOURCE_DIR}/third_party/botan/configure.py --prefix=${BOTAN_INSTALL_DIR}
		)
	message("botan install dir: ${BOTAN_INSTALL_DIR}")
	#XXX maybe I should set PKG_CONFIG_PATH then use pkg_check_module
	set (BOTAN_INCLUDE_DIRS "${BOTAN_INSTALL_DIR}/include/botan-2")
	set (BOTAN_LIBRARIES "${BOTAN_INSTALL_DIR}/lib/libbotan-2.a") # statically link
endif (USE_SYSTEM_BOTAN)

if (BUILD_WEB_SERVICE)
option (USE_SYSTEM_GRPC "use system installed grpc(with protobuf)" OFF)
if (USE_SYSTEM_GRPC)
	# XXX grpc depends on specific protobuf api, so wo restrict the version to avoid compilation error
	pkg_check_modules (GRPC REQUIRED grpc=1.4.0-dev)
	pkg_check_modules (PROTOBUF REQUIRED protobuf=3.2.0)
else ()
	# TODO check `make` command
	# TODO split grpc and protobuf dependency
	include (ExternalProject)
	set(GRPC_INSTALL_DIR "${PROJECT_BINARY_DIR}/third_party")
	set(GRPC_MAKE_ARGS -C ${PROJECT_SOURCE_DIR}/third_party/grpc \\
		 -e prefix=${GRPC_INSTALL_DIR} \\
		# BINARY_DIR "${GRPC_INSTALL_DIR}/src/grpc-build"
	       	 -e BUILDDIR=${GRPC_INSTALL_DIR}/src/grpc-build \\
		 -e HAS_SYSTEM_PROTOBUF=false \\
		 -e HAS_VALID_PROTOC=false \\
		 PROTOBUF_CONFIG_OPTS='--prefix=${GRPC_INSTALL_DIR}'
		 )
	ExternalProject_Add (grpc
		PREFIX ${GRPC_INSTALL_DIR}
		GIT_REPOSITORY "https://github.com/grpc/grpc.git"
		GIT_TAG "90bb7bddd138bac7126b01bfcb31fa9beff7d2fb"
		# XXX when I defined GIT_TAG, GIT_SUBMODULES will not work...
		UPDATE_COMMAND git submodule init && git submodule update && git submodule sync
		SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/grpc"
		CONFIGURE_COMMAND ""
		# BINARY_DIR "${GRPC_INSTALL_DIR}/src/grpc-build"
		BUILD_COMMAND make ${GRPC_MAKE_ARGS}
		INSTALL_COMMAND make ${GRPC_MAKE_ARGS} install
		COMMAND make -C ${PROJECT_SOURCE_DIR}/third_party/grpc/third_party/protobuf install
		# FIXME seems not work, I want to update PKG_CONFIG_PATH...
		COMMAND export PKG_CONFIG_PATH=${GRPC_INSTALL_DIR}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}
		COMMAND export PATH=${GRPC_INSTALL_DIR}/bin:$ENV{PATH}
		)
	# XXX How to update PKG_CONFIG_PATH...
	message("grpc install dir: ${GRPC_INSTALL_DIR}")
	set (GRPC_INCLUDE_DIRS "${GRPC_INSTALL_DIR}/include")
	set (GRPC_LIBRARIES ${GRPC_INSTALL_DIR}/lib/libgrpc++.a ${GRPC_INSTALL_DIR}/lib/libgrpc.a -lz -lcares -lssl -ldl -lcrypto)
	set (PROTOBUF_INCLUDE_DIRS "${GRPC_INSTALL_DIR}/include")
	set (PROTOBUF_LIBRARIES ${GRPC_INSTALL_DIR}/lib/libprotobuf.a -pthread -lpthread -lz)
endif (USE_SYSTEM_GRPC)
endif (BUILD_WEB_SERVICE)

pkg_check_modules (SQLITE3 REQUIRED sqlite3>=3.8.7)

## Set Compilation Database for YouCompleteMe
# \ref http://bastian.rieck.ru/blog/posts/2015/ycm_cmake/
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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



