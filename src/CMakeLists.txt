add_executable ( pbook
	main.cpp
	cmdSeparator.cpp
	passwordGenerator.cpp
	randomTool.cpp
	regextool.cpp
	saveConfig.cpp
	stringTool.cpp
	)

add_library ( pbook.crypto
	crypto.cpp
	)

include_directories ( pbook.crypto ${BOTAN_INCLUDE_DIRS} )
target_link_libraries ( pbook.crypto ${BOTAN_LIBRARIES} )

include_directories ( pbook ${Boost_INCLUDE_DIRS} )
target_link_libraries ( pbook
	pbook.crypto
	m ${LIB_READLINE} ${Boost_LIBRARIES}
	)

install (TARGETS pbook
	RUNTIME DESTINATION  bin
	)

install ( TARGETS pbook.crypto
	LIBRARY DESTINATION  lib
	ARCHIVE DESTINATION  lib
	)
