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
    typedef std::vector<uint8_t> bytes;
    const std::string VERSION = "1.0.0";
    class PasswordManager
    {
      public:
        //~PasswordManager();
        virtual int32_t openDB(const std::string &filename) = 0;
        virtual int32_t showIndexByDomain(const std::string &domain) = 0;
        virtual bytes searchByIndex(int32_t index) = 0;
        virtual int32_t addPasswd(bytes passwd, const std::string &domain) = 0;
        virtual int32_t deleteByIndex(int32_t index) = 0;
        virtual std::vector<std::string> getDomainLists() = 0;
        virtual std::string version() { return VERSION; }
    };

    class PasswordManager_SQLite3 : public PasswordManager
    {
      public:
        PasswordManager_SQLite3();
        inline void setEncVer(const std::string encVer) { _encVer = encVer; }
        inline std::string getEncVer() { return _encVer; }
        int32_t openDB(const std::string &filename);
        int32_t showIndexByDomain(const std::string &domain);
        bytes searchByIndex(int32_t index);
        int32_t addPasswd(bytes passwd, const std::string &domain);
        int32_t deleteByIndex(int32_t index);
        std::vector<std::string> getDomainLists();

      private:
        int32_t _initDB();
        // int32_t _checkDB();
        char **_getQuery(const char *sqlRequest, int32_t *pnRow = NULL, int32_t *qnRow = NULL);
        sqlite3 *_conn;
        std::ostream *_os;
        std::string _encVer;
    };
}

#endif  // _PASSWORDMANAGER_H
