include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_crypto test_crypto.cpp)
target_link_libraries(test_crypto pbook.crypto ${GTEST_BOTH_LIBRARIES})
#GTEST_ADD_TESTS(test_crypto "" test_crypto.cpp)
add_test(AllTests test_crypto)

