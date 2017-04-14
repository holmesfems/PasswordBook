add_library ( pbook.crypto
	crypto.cpp
	)

if (NOT USE_SYSTEM_BOTAN)
	add_dependencies(pbook.crypto botan)
endif ()

include_directories ( pbook.crypto ${BOTAN_INCLUDE_DIRS} )
target_link_libraries ( pbook.crypto ${BOTAN_LIBRARIES} )

add_library ( pbook.db
	passwordManager.cpp
	)
include_directories ( pbook.db ${SQLITE3_INCLUDE_DIRS} )
target_link_libraries ( pbook.db ${SQLITE3_LIBRARIES} )

add_library ( pbook.libs
	miscUtil.cpp
	cmdSeparator.cpp
	passwordGenerator.cpp
	randomTool.cpp
	regextool.cpp
	saveConfig.cpp
	stringTool.cpp
	)
include_directories ( pbook.libs ${Boost_INCLUDE_DIRS} )
target_link_libraries ( pbook.libs
	m
	${Boost_LIBRARIES}
	pbook.crypto pbook.db
	)

add_executable ( pbook
	main.cpp
	)
target_link_libraries ( pbook
	${LIB_READLINE}
	pbook.libs
	)

install (TARGETS pbook
	RUNTIME DESTINATION  bin
	)

install ( TARGETS pbook.crypto pbook.db
	LIBRARY DESTINATION  lib
	ARCHIVE DESTINATION  lib
	)
