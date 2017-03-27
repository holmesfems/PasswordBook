/*
 * passwordManager.cpp
 * Created on 2017/03/27 14:38:22
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "passwordManager.h"
#include <boost/filesystem.hpp>

namespace PasswordManager
{
    PasswordManager::PasswordManager() { conn = NULL; }
    int PasswordManager::openDB(const std::string &filename) {}
}
