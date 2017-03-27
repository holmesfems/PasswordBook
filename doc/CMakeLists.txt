
# \ref https://majewsky.wordpress.com/2010/08/14/tip-of-the-day-cmake-and-doxygen/
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen/
		DESTINATION share/${CMAKE_PROJECT_NAME}/doc
		COMPONENT docs)
else(DOXYGEN_FOUND)
	message (WARNING "doxygen not found, cannot build doc")
endif(DOXYGEN_FOUND)
