add_executable ( password_service 
	password_service.cc
	)

if (NOT USE_SYSTEM_GRPC)
	add_dependencies(password_service grpc)
	set (GRPC_CPP_PLUGIN ${GRPC_INSTALL_DIR}/bin/grpc_cpp_plugin)
	# XXX very ugly method to find protoc, grpc_cpp_plugin
	find_program(PROTOBUF_EXE
		protoc
		HINTS ${GRPC_INSTALL_DIR}/bin ENV PATH
		)
else ()
	include (FindProtobuf)
	if (PROTOBUF_FOUND)
		set (PROTOBUF_EXE ${PROTOBUF_PROTOC_EXECUTABLE})
	else ()
		find_program(PROTOBUF_EXE
			protoc
			)
	endif (PROTOBUF_FOUND)
	find_program(GRPC_CPP_PLUGIN
		grpc_cpp_plugin
		)
endif ()

set (PROTO_FILES password_service.proto)
set (PROTO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})
# ref FindProtobuf.cmake
foreach (FIL ${PROTO_FILES})
	message ("protoc ${FIL}")
	get_filename_component(FIL_P ${FIL} NAME_WE)
	get_filename_component(FIL_A ${FIL} ABSOLUTE) 
	list (APPEND PROTO_SRC "${FIL_P}.pb.cc")
	list (APPEND PROTO_SRC "${FIL_P}.grpc.pb.cc")
	list (APPEND PROTO_HDR "${FIL_P}.pb.h")
	list (APPEND PROTO_HDR "${FIL_P}.grpc.pb.h")
	add_custom_command(
		OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_P}.pb.cc"
		"${CMAKE_CURRENT_BINARY_DIR}/${FIL_P}.pb.h"
		"${CMAKE_CURRENT_BINARY_DIR}/${FIL_P}.grpc.pb.cc"
		"${CMAKE_CURRENT_BINARY_DIR}/${FIL_P}.grpc.pb.h"
		COMMAND ${PROTOBUF_EXE}
		ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR} --proto_path=${PROTO_INCLUDE} ${FIL_A}
		COMMAND ${PROTOBUF_EXE}
		ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN} --proto_path=${PROTO_INCLUDE} ${FIL_A}
		# XXX maybe helpful to some ide...
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${FIL_P}.pb.cc" ${CMAKE_CURRENT_SOURCE_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${FIL_P}.pb.h" ${CMAKE_CURRENT_SOURCE_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${FIL_P}.grpc.pb.cc" ${CMAKE_CURRENT_SOURCE_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${FIL_P}.grpc.pb.h" ${CMAKE_CURRENT_SOURCE_DIR}
		DEPENDS ${FIL_A} ${PROTOBUF_EXE}
		COMMENT "Running c++ protocol buffer compiler on ${FIL}"
		VERBATIM
		)
	set_source_files_properties(${PROTO_SRC} ${PROTO_HDR} PROPERTIES GENERATED TRUE) 
endforeach ()

add_library (password_service.grpc_proto
	${PROTO_SRC}
	${PROTO_HDR}
	)

include_directories (${GRPC_INCLUDE_DIRS})
include_directories (${PROTOBUF_INCLUDE_DIRS})
include_directories (${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries (password_service.grpc_proto ${GRPC_LIBRARIES})
target_link_libraries (password_service.grpc_proto ${PROTOBUF_LIBRARIES})

target_link_libraries (password_service password_service.grpc_proto)
target_link_libraries (password_service pbook.libs)

install (TARGETS password_service password_service
	RUNTIME DESTINATION bin
	)
