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
        //~PasswordManager();
        virtual int openDB(const std::string &filename)=0;
        virtual int showIndexByDomain(const std::string &domain)=0;
        virtual std::string searchByIndex(int index)=0;
        virtual int addPasswd(const std::string &passwdStr, const std::string &domain)=0;
        virtual int deleteByIndex(int index)=0;
        virtual std::vector<std::string> getDomainLists()=0;
    };

    class PasswordManager_SQLite3:public PasswordManager
    {
      public:
        PasswordManager_SQLite3();
        int openDB(const std::string &filename);
        int showIndexByDomain(const std::string &domain);
        std::string searchByIndex(int index);
        int addPasswd(const std::string &passwdStr, const std::string &domain);
        int deleteByIndex(int index);
        std::vector<std::string> getDomainLists();
      private:
        int _initDB();
        // int _checkDB();
        char **_getQuery(const char *sqlRequest, int *pnRow = NULL, int *qnRow = NULL);
        sqlite3 *_conn;
        std::ostream *_os;
    };
}

#endif  // _PASSWORDMANAGER_H
