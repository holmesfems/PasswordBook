/*
 * crypto.h
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

/*! \file crypto.h
 *  \brief wrap cryptographic functions
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

    std::string unpad(const bytes &input);

    /*! \brief encrypt password
     *
     *  SAVE_VALUE := encrypt(PWD0, PWD_OUTPUT)
     *  PWD_OUTPUT := decrypt(PWD0, SAVE_VALUE)
     *  SAVE_VALUE is {salt_len, salt, IV, Encrypted}
     *
     *  user input: PWD0
     *  key := PBKDF2(PWD0, salt)
     *  encrypted := aes-256-cbc-pkcs7_encrypt(key, data, iv)
     *  save: {salt_len, salt, iv, encrypted}
     *
     *  \param pwd_master master password (PWD0)
     *  \param to_encrypt text to be encrypted
     *  \return encrypted data {PWD0_Salt_len, PWD0_Salt, IV, Encrypted_Pwd}
     *  \TODO consider about compatibility when encrypt algorithm update
     */
    bytes encrypt(const std::string &pwd_master, const std::string &to_encrypt);

    /*! \brief decrypt to get password
     *
     * load: save
     * parse into {salt_len, salt, iv, encrypted}
     * key := PBKDF2(PWD0, salt)
     * data := decrypt(key, encrypted, iv)
     *
     *  \param pwd_master master password (PWD0)
     *  \param saved_data data to be decrypted
     *  \return original password string
     */
    std::string decrypt(const std::string &pwd_master, const bytes &saved_data);
    std::string decrypt(const std::string &pwd_master, const uint8_t *, size_t);
}

#endif /* !CRYPTO_H */
/* vim: set et: */
