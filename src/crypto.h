/*
 * crypto.h
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CRYPTO_H
#define CRYPTO_H

#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <vector>

namespace Crypto
{
    typedef std::vector<uint8_t> bytes;

    /*! \file crypto.h
     *  \brief wrap cryptographic functions
     */

    /*! \brief pad string to target length
     *
     *  For example: pad("12", 4) => {0x31, 0x32, 0x00, 0x02}
     *  \ref http://www.di-mgt.com.au/cryptopad.html
     *  \note pwd_str.size() < target_len (or 255)
     *
     *  \param pwd_str input string
     *  \param target_len target padded length
     *  \return padded vector of byte
     */
    bytes pad(const std::string &pwd_str, uint32_t target_len = 32);
    /*
     * SAVE_VALUE := Gen_Enc(PWD0, PWD_OUTPUT)
     * PWD_OUTPUT := Gen_Dec(PWD0, SAVE_VALUE)
     */
}

#endif /* !CRYPTO_H */
/* vim: set et: */
