/*
 * test_crypto.cpp
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

// https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
#include <gtest/gtest.h>

#include <iostream>

#include "../src/crypto.h"

namespace
{
    TEST(CryptoPadTest, BaseCheck)
    {
        std::vector<uint8_t> data = {0x31, 0x32, 0x00, 0x02};
        ASSERT_EQ(data, Crypto::pad("12", 4));
    }
}

int main(int argc, char **argv)
{
    std::cout << "running google test..." << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/* vim: set et: */
