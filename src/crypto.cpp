/*
 * crypto.cpp
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "crypto.h"

#include <botan/botan.h>
#include <botan/cipher_mode.h>
#include <botan/hex.h>
#include <botan/mode_pad.h>
#include <botan/pbkdf.h>

static const char ENC_METHOD[] = "AES-256/CBC/PKCS7";
static const size_t SALT_ITER_COUNT = 2333;

namespace Crypto {
// TODO use try-catch block to handle exception

bytes pad(const std::string &pwd_str, uint32_t target_len) {
  assert(pwd_str.size() < target_len && pwd_str.size() < 0xff);
  bytes output(pwd_str.begin(), pwd_str.end());
  output.resize(target_len, 0);
  output.at(target_len - 1) = static_cast<uint8_t>(pwd_str.size());
  return output; // TODO use shared_ptr or move
}

std::string unpad(const bytes &input) {
  size_t str_len = input.back();
  std::string output(input.begin(), input.begin() + str_len);
  return output;
}

// ref https://botan.randombit.net/manual/symmetric_crypto.html#id1
bytes encrypt(const std::string &pwd_master, const std::string &to_encrypt) {
  Botan::secure_vector<uint8_t> plain_text(
      to_encrypt.data(), to_encrypt.data() + to_encrypt.length());
  std::unique_ptr<Botan::RandomNumberGenerator> rng(new Botan::AutoSeeded_RNG);

  // https://botan.randombit.net/manual/pbkdf.html#_CPPv2NK5PBKDF10derive_keyE6size_tRKNSt6stringEPK4byte6size_t6size_t
  std::unique_ptr<Botan::PBKDF> pbkdf(Botan::get_pbkdf("PBKDF2(SHA-256)"));
  uint8_t salt_len = 16;
  Botan::secure_vector<Botan::byte> salt = rng->random_vec(salt_len);
  // generate persuado random key based on pwd_master
  Botan::OctetString master_key =
      pbkdf->derive_key(32, pwd_master, &salt[0], salt.size(), SALT_ITER_COUNT);

  // use AES-256 CBC mode, PKCS#7 padding
  std::unique_ptr<Botan::Cipher_Mode> enc(
      Botan::get_cipher_mode(ENC_METHOD, Botan::ENCRYPTION));
  enc->set_key(master_key);

  // generate randomized IV (initial_vector)
  bytes initial_vector(enc->default_nonce_length());
  rng->randomize(initial_vector.data(), initial_vector.size());

  enc->start(initial_vector);
  enc->finish(plain_text);

#ifdef DEBUG
  std::cout << "encrypt: " << to_encrypt
            << ", to: " << Botan::hex_encode(plain_text)
            << ", method: " << enc->name()
            << ", iv:" << Botan::hex_encode(initial_vector) << std::endl;
#endif
  bytes output = { salt_len };
  output.insert(output.end(), std::make_move_iterator(salt.begin()),
                std::make_move_iterator(salt.end()));
  output.insert(output.end(), std::make_move_iterator(initial_vector.begin()),
                std::make_move_iterator(initial_vector.end()));
  output.insert(output.end(), std::make_move_iterator(plain_text.begin()),
                std::make_move_iterator(plain_text.end()));
  return output;
}

std::string decrypt(const std::string &pwd_master, const bytes &saved_data) {
  return decrypt(pwd_master, saved_data.data(), saved_data.size());
}
std::string decrypt(const std::string &pwd_master, const uint8_t *saved_data,
                    size_t saved_len) {
  uint8_t salt_len = saved_data[0]; // extract salt length
  const uint8_t *cur_head = saved_data + 1;
  size_t cur_len = saved_len - 1;

  // extract master_key with salt
  std::unique_ptr<Botan::PBKDF> pbkdf(Botan::get_pbkdf("PBKDF2(SHA-256)"));
  Botan::OctetString master_key =
      pbkdf->derive_key(32, pwd_master, cur_head, salt_len, SALT_ITER_COUNT);

  cur_head += salt_len;
  cur_len -= salt_len;

  std::unique_ptr<Botan::Cipher_Mode> enc(
      Botan::get_cipher_mode(ENC_METHOD, Botan::DECRYPTION));
  enc->set_key(master_key);
  enc->start(cur_head, enc->default_nonce_length());

  const uint8_t *encrypted_head = cur_head + enc->default_nonce_length();
  size_t encrypted_len = cur_len - enc->default_nonce_length();
  Botan::secure_vector<uint8_t> encrypted(encrypted_head,
                                          encrypted_head + encrypted_len);
  enc->finish(encrypted);
#ifdef DEBUG
  std::cout << "decrypt: " << Botan::hex_encode(encrypted_head, encrypted_len)
            << ",to: " << Botan::hex_encode(encrypted)
            << ", method: " << enc->name() << ", iv:"
            << Botan::hex_encode(saved_data, enc->default_nonce_length())
            << std::endl;
#endif
  return std::string(std::make_move_iterator(encrypted.begin()),
                     std::make_move_iterator(encrypted.end()));
}
}

/* vim: set et: */
