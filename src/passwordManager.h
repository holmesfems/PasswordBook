/*
 * passwordManager.h
 * Created on 2017/03/25 09:53:44
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef _PASSWORDMANAGER_H
#define _PASSWORDMANAGER_H
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
namespace PasswordManager
{
    class PasswordManager
    {
      public:
        PasswordManager();
        //~PasswordManager();
        int openDB(const std::string& filename);
        int dbinit();
        std::string searchByDomain(const std::string& domain);
        std::string searchByIndex(int index);
        int addPasswd(const std::string& filename, const std::string& domain);
        int deleteByDomain(const std::string& domain);
        int deleteByIndex(int index);
        std::vector<std::string> getDomainLists();
        std::string getDomainByRegex(const std::string& regexStr);

      private:
        sqlite3* conn;
    };
}

#endif  // _PASSWORDMANAGER_H
