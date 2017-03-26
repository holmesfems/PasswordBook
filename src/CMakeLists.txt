add_executable ( pbook
	main.cpp
	cmdSeparator.cpp
	passwordGenerator.cpp
	randomTool.cpp
	regextool.cpp
	saveConfig.cpp
	stringTool.cpp
	)

include_directories ( pbook ${Boost_INCLUDE_DIRS} )
target_link_libraries ( pbook m ${LIB_READLINE} ${Boost_LIBRARIES})

install (TARGETS pbook
	RUNTIME DESTINATION  bin
	)
