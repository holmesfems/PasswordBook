/*
 * crypto.cpp
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "crypto.h"

namespace Crypto
{
    bytes pad(const std::string &pwd_str, uint32_t target_len)
    {
        assert(pwd_str.size() < target_len && pwd_str.size() < 0xff);
        bytes output(pwd_str.begin(), pwd_str.end());
        output.resize(target_len, 0);
        output.at(target_len - 1) = static_cast<uint8_t>(pwd_str.size());
        return output;  // TODO use shared_ptr or move
    }
}

/* vim: set et: */
