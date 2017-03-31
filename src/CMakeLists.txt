add_library ( pbook.crypto
	crypto.cpp
	)
include_directories ( pbook.crypto ${BOTAN_INCLUDE_DIRS} )
target_link_libraries ( pbook.crypto ${BOTAN_LIBRARIES} )

add_library ( pbook.db
	passwordManager.cpp
	)
include_directories ( pbook.db ${SQLITE3_INCLUDE_DIRS} )
target_link_libraries ( pbook.db ${SQLITE3_LIBRARIES} )

add_executable ( pbook
	main.cpp
	miscUtil.cpp
	cmdSeparator.cpp
	passwordGenerator.cpp
	randomTool.cpp
	regextool.cpp
	saveConfig.cpp
	stringTool.cpp
	)
include_directories ( pbook ${Boost_INCLUDE_DIRS} )
target_link_libraries ( pbook
	m ${LIB_READLINE} ${Boost_LIBRARIES}
	pbook.crypto pbook.db
	)

install (TARGETS pbook
	RUNTIME DESTINATION  bin
	)

install ( TARGETS pbook.crypto pbook.db
	LIBRARY DESTINATION  lib
	ARCHIVE DESTINATION  lib
	)
