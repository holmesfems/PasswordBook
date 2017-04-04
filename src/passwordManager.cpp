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
#include "stringTool.h"

namespace PasswordManager
{
    PasswordManager *FactoryCreateManager(const std::string &dbtype)
    {
        if (dbtype == "sqlite3") {
            return new PasswordManager_SQLite3();
        }
        return NULL;
    }

    PasswordManager_SQLite3::PasswordManager_SQLite3()
    {
        _conn = NULL;
        _os = &(std::cout);

        sqlite3_config(SQLITE_CONFIG_LOG, &PasswordManager_SQLite3::errorLogCallback, NULL);
    }

    int32_t PasswordManager_SQLite3::_initDB()
    {
        char *errMsg = NULL;
        int32_t err = 0;
        std::ostringstream sql;
        sql << R"(CREATE TABLE IF NOT EXISTS )" << TABLENAME << "(";
        int8_t is_first = 1;
        for (auto item : COLUMN) {
            if (!is_first)
                sql << ",";
            else
                is_first = 0;
            sql << item.name << " ";
            sql << item.type << " ";
            sql << item.init;
        }
        sql << ");";
        err = sqlite3_exec(_conn, sql.str().c_str(), NULL, NULL, &errMsg);
        if (err != SQLITE_OK) {
            (*_os) << errMsg << std::endl;
            sqlite3_free(errMsg);
            errMsg = NULL;
            return err;
        }
        (*_os) << "Open table '" << TABLENAME << "'" << std::endl;
        return 0;
    }

    int32_t PasswordManager_SQLite3::openDB(const std::string &filename)
    {
        //(*_os) << filename << std::endl;
        int32_t err = sqlite3_open(filename.c_str(), &_conn);
        if (err != SQLITE_OK) {
            (*_os) << "An error occured while open database file: " << filename
                   << ".  error_code: " << err << " " << sqlite3_errstr(err) << std::endl;
            return err;
        }
        return _initDB();
    }

    int32_t PasswordManager_SQLite3::showIndexByDomain(const std::string &domain)
    {
        std::ostringstream sql;
        sql << "SELECT " << COLUMN[0].name << "," << COLUMN[1].name << " FROM " << TABLENAME
            << " WHERE " << COLUMN[1].name << R"( LIKE '%)" << domain << R"(%';)";
        sqlite3_stmt *pstmt;
        int32_t rc;
        rc = sqlite3_prepare(_conn, sql.str().c_str(), -1, &pstmt, NULL);
        if (rc != SQLITE_OK) {
            (*_os) << "An error occured while selecting table" << std::endl;
            return -1;
        }
        std::vector<int32_t> newidList;
        std::vector<std::string> newdomainList;
        while ((rc = sqlite3_step(pstmt)) == SQLITE_ROW) {
            newidList.push_back(sqlite3_column_int(pstmt, 0));
            newdomainList.push_back((const char *)(sqlite3_column_text(pstmt, 1)));
        }
        if (newidList.size() > 0) {
            (*_os) << "Hit " << newidList.size() << " items:" << std::endl;
            size_t i;
            for (i = 0; i < newidList.size(); i++) {
                (*_os) << newidList[i] << "\t" << newdomainList[i] << std::endl;
            }
            _idlist = newidList;
        } else {
            (*_os) << "Hit no item" << std::endl;
        }
        rc = sqlite3_finalize(pstmt);
        return newidList.size();
    }  // showIndexByDomain()

    bytes PasswordManager_SQLite3::searchByIndex(int32_t id)
    {
        std::ostringstream sql;
        sql << "SELECT " << COLUMN[2].name << "," << COLUMN[3].name << " FROM " << TABLENAME
            << " WHERE " << COLUMN[0].name << " = ?";
        sqlite3_stmt *pstmt;
        int32_t rc;
        int32_t nblob;
        bytes ret;
        rc = sqlite3_prepare(_conn, sql.str().c_str(), -1, &pstmt, NULL);
        if (rc != SQLITE_OK) {
            (*_os) << "An error occured while selecting table" << std::endl;
            return ret;
        }
        sqlite3_bind_int(pstmt, 1, id);
        rc = sqlite3_step(pstmt);
        if (rc == SQLITE_ROW) {
            nblob = sqlite3_column_bytes(pstmt, 0);
            const uint8_t *zblob = (const uint8_t *)(sqlite3_column_blob(pstmt, 0));
            ret = bytes(zblob, zblob + nblob);
            _encVer = (const char *)(sqlite3_column_text(pstmt, 1));
        }
        rc = sqlite3_finalize(pstmt);
        return ret;
    }

    int32_t PasswordManager_SQLite3::addPasswd(bytes passwd, const std::string &domain)
    {
        std::ostringstream sql;
        sql << "INSERT OR REPLACE INTO " << TABLENAME << "(" << COLUMN[1].name << ","
            << COLUMN[2].name << "," << COLUMN[3].name << ") VALUES (?,?,?)";
        int32_t rc;
        sqlite3_stmt *pstmt;
        rc = sqlite3_prepare(_conn, sql.str().c_str(), -1, &pstmt, NULL);
        if (rc != SQLITE_OK) {
            (*_os) << "An error occured when inserting table value" << std::endl;
            return rc;
        }
        sqlite3_bind_text(pstmt, 1, domain.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_blob(pstmt, 2, passwd.data(), passwd.size(), SQLITE_STATIC);
        sqlite3_bind_text(pstmt, 3, _encVer.c_str(), -1, SQLITE_STATIC);
        rc = sqlite3_step(pstmt);
        assert(rc != SQLITE_ROW);
        rc = sqlite3_finalize(pstmt);
        return rc;
    }

    bytes PasswordManager_SQLite3::searchByDomain(const std::string &domain)
    {
        std::ostringstream dummy;
        std::ostream *temp = _os;
        _os = &dummy;
        bytes ret;
        if (showIndexByDomain(domain) > 0) {
            ret = searchByIndex(_idlist[0]);
        }
        _os = temp;
        return ret;
    }

    int32_t PasswordManager_SQLite3::deleteByIndex(int32_t id)
    {
        std::ostringstream sql;
        sql << "DELETE FROM " << TABLENAME << " WHERE " << COLUMN[0].name << "=?;";
        sqlite3_stmt *pstmt;
        int rc;
        rc = sqlite3_prepare(_conn, sql.str().c_str(), -1, &pstmt, NULL);
        if (rc != SQLITE_OK) {
            (*_os) << "An error occured while deleting item" << std::endl;
            return rc;
        }
        sqlite3_bind_int(pstmt, 1, id);
        rc = sqlite3_step(pstmt);
        if (rc != SQLITE_ROW) {
            (*_os) << "Nothing deleted" << std::endl;
        } else {
            (*_os) << "Succeed to delete item id:" << id << std::endl;
        }
        rc = sqlite3_finalize(pstmt);
        return 0;
    }

    std::vector<std::string> PasswordManager_SQLite3::getDomainList()
    {
        std::ostringstream sql;
        sql << "SELECT " << COLUMN[0].name << " , " << COLUMN[1].name << " FROM " << TABLENAME
            << ";";
        sqlite3_stmt *pstmt;
        int32_t rc;
        rc = sqlite3_prepare(_conn, sql.str().c_str(), -1, &pstmt, NULL);
        if (rc != SQLITE_OK) {
            (*_os) << "An error occured while selecting table" << std::endl;
            return {};
        }
        std::vector<std::string> newdomainList;
        std::vector<int32_t> newidlist;
        while ((rc = sqlite3_step(pstmt)) == SQLITE_ROW) {
            newdomainList.push_back((const char *)(sqlite3_column_text(pstmt, 1)));
            newidlist.push_back(sqlite3_column_int(pstmt, 0));
        }
        if (newdomainList.size() > 0) {
            (*_os) << "Hit " << newdomainList.size() << " items:" << std::endl;
            size_t i;
            (*_os) << COLUMN[0].name << "\t" << COLUMN[1].name << std::endl;
            for (i = 0; i < newdomainList.size(); i++) {
                (*_os) << newidlist[i] << "\t" << newdomainList[i] << std::endl;
            }
        } else {
            (*_os) << "Hit no item" << std::endl;
        }
        rc = sqlite3_finalize(pstmt);
        return newdomainList;
    }

    void PasswordManager_SQLite3::errorLogCallback(void *pArg, int iErrCode, const char *zMsg)
    {
        (void)pArg;
        std::cerr << "(" << iErrCode << ") " << zMsg << std::endl;
    }
}

/* vim: set et: */
