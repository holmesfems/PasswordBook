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

    TEST(CryptoEncryptTest, BaseCheck)
    {
        // TODO give random input
        const std::string pwd = "PWD0";
        const std::string pwd_output = "pwdoutputx";
        Crypto::bytes save_value = Crypto::encrypt(pwd, pwd_output);
        std::cout << save_value.size() << std::endl;
        for ( auto &i : save_value ) {
            std::cout << "0x" << std::hex << (int)i << " " << std::dec;
        }
        std::cout << std::endl;
        ASSERT_NE(Crypto::encrypt(pwd, pwd_output), Crypto::encrypt(pwd, pwd_output));
        std::string load_passwd = Crypto::decrypt(pwd, save_value);
        std::cout << "get string: " << load_passwd << std::endl;
        ASSERT_EQ(pwd_output, load_passwd);
    }
}

int main(int argc, char **argv)
{
    std::cout << "running google test..." << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/* vim: set et: */
