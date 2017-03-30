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
        int showIndexByDomain(const std::string& domain);
        std::string searchByIndex(int index);
        int addPasswd(const std::string& passwdStr, const std::string& domain);
        int changePasswd(const std::string& passwdStr,int index);
        int deleteByIndex(int index);
        std::vector<std::string> getDomainLists();

      private:
        int _initDB();
        //int _checkDB();
        char **_getQuery(const char *sqlRequest,int *pnRow=NULL,int *qnRow=NULL);
        sqlite3* _conn;
        std::ostream *_os;
    };
}

#endif  // _PASSWORDMANAGER_H
