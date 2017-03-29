/*
 * passwordManager.cpp
 * Created on 2017/03/27 14:38:22
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "passwordManager.h"
#include <boost/filesystem.hpp>
#include <sstream>

namespace PasswordManager
{
    PasswordManager::PasswordManager() { _conn = NULL; }

    int PasswordManager::_initDB()
    {
        char *errMsg=NULL;
        int err=0;
        err=sqlite3_exec(_conn,R"(CREATE TABLE IF NOT EXISTS table1
            (index INTEGER PRIMARY KEY AUTOINCREMENT,domain TEXT NOT NULL,pwdoutput TEXT NOT NULL))"
                ,NULL,NULL,&errMsg);
        if(err != SQLITE_OK){
            (*_os) << errMsg << std::endl;
            sqlite3_free(errMsg);
            errMsg=NULL;
            return err;
        }
        (*_os) << "Open table 'table1'" << std::endl;
        return 0;
    }

    char ** PasswordManager::_getQuery(const char *sqlRequest,int *pnRow,int *pnCol)
    {
        char **query=NULL;
        char *errMsg=NULL;
        int err;
        err=sqlite3_get_table(_conn,
            sqlRequest,&query,
            pnRow,pnCol,&errMsg);
        if(err!=SQLITE_OK)
        {
            (*_os) << errMsg << std::endl;
            sqlite3_free(errMsg);
            errMsg=NULL;
            return NULL;
        }
        return query;
    }

    int PasswordManager::openDB(const std::string &filename)
    {
        int err=sqlite3_open_v2(filename.c_str(),&_conn,SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,NULL);
        if(err!=SQLITE_OK)
        {
            (*_os) << "An error occured while open database file:" << filename << std::endl;
            return err;
        }
        _initDB();
        //_checkDB();
        return 0;
    }

    int PasswordManager::showIndexByDomain(const std::string &domain)
    {
        int pnRow,pnCol;
        std::ostringstream sql;
        sql << R"(SELECT index,domain FROM table1 WHERE domain LIKE '%)" << domain << R"(%')";
        char **query = _getQuery(sql.str().c_str(),&pnRow,&pnCol);
        if(query == NULL){
            (*_os) << "An error occured while searching domain: " << domain << std::endl;
             return -1;
        }
        if(pnRow == 0){
            (*_os) << "No one is hit" << std::endl;
        }else{
            (*_os) << "Hit " << pnRow << " Results:" << std::endl;
            int i,j;
            for(i=0;i<=pnRow;i++)
            {
                for(j=0;j<pnCol;j++)
                {
                    if(j>0) (*_os) << '\t';
                    (*_os) << query[i*pnCol+j];
                }
                (*_os) << std::endl;
            }
        }
        return 0;
    }
}
