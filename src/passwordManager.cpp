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
        return nullptr;
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
        // TODO change 'table1' to macro or configurable variable
        err = sqlite3_exec(_conn, R"(CREATE TABLE IF NOT EXISTS table1
            (id INTEGER PRIMARY KEY AUTOINCREMENT,
             domain TEXT UNIQUE ON CONFLICT REPLACE,
             pwdoutput BLOB NOT NULL,
             encver TEXT NOT NULL))",
                           NULL, NULL, &errMsg);
        if (err != SQLITE_OK) {
            (*_os) << errMsg << std::endl;
            sqlite3_free(errMsg);
            errMsg = NULL;
            return err;
        }
        (*_os) << "Open table 'table1'" << std::endl;
        return 0;
    }

    char **PasswordManager_SQLite3::_getQuery(const char *sqlRequest, int32_t *pnRow,
                                              int32_t *pnCol)
    {
        char **query = NULL;
        char *errMsg = NULL;
        int32_t err;
        err = sqlite3_get_table(_conn, sqlRequest, &query, pnRow, pnCol, &errMsg);
        if (err != SQLITE_OK) {
            (*_os) << errMsg << std::endl;
            sqlite3_free(errMsg);
            errMsg = NULL;
            (*pnRow) = ((*pnCol) = -1);
            return NULL;
        }
        return query;
    }

    int32_t PasswordManager_SQLite3::openDB(const std::string &filename)
    {
        int32_t err = sqlite3_open_v2(filename.c_str(), &_conn,
                                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (err != SQLITE_OK) {
            (*_os) << "An error occured while open database file: " << filename
                   << ".  error_code: " << err << " " << sqlite3_errstr(err) << std::endl;
            return err;
        }
        return _initDB();
    }

    int32_t PasswordManager_SQLite3::showIndexByDomain(const std::string &domain)
    {
        int32_t pnRow, pnCol;
        std::ostringstream sql;
        sql << R"(SELECT id,domain FROM table1 WHERE domain LIKE '%)" << domain << R"(%')";
        char **query = _getQuery(sql.str().c_str(), &pnRow, &pnCol);
        if (query == NULL) {
            (*_os) << "An error occured while searching domain: " << domain << std::endl;
            return -1;
        }
        if (pnRow == 0) {
            (*_os) << "No one is hit" << std::endl;
        } else {
            (*_os) << "Hit " << pnRow << " Results:" << std::endl;
            int32_t i, j;
            for (i = 0; i <= pnRow; i++) {
                for (j = 0; j < pnCol; j++) {
                    if (j > 0) (*_os) << '\t';
                    (*_os) << query[i * pnCol + j];
                }
                (*_os) << std::endl;
            }
        }
        sqlite3_free_table(query);
        query = NULL;
        return 0;
    }  // showIndexByDomain()

    bytes PasswordManager_SQLite3::searchByIndex(int32_t id)
    {
        std::ostringstream sql;
        sql << R"(SELECT pwdoutput,encver FROM table1 WHERE id = ?)";
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
            _encVer = (const char *)(sqlite3_column_text(pstmt, 0));
        }
        rc = sqlite3_finalize(pstmt);
        return ret;
    }

    int32_t PasswordManager_SQLite3::addPasswd(bytes passwd, const std::string &domain)
    {
        std::ostringstream sql;
        sql << R"(INSERT OR REPLACE INTO table1 (domain,pwdoutput,encver) VALUES(?,?,?))";
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

    int32_t PasswordManager_SQLite3::deleteByIndex(int32_t id)
    {
        // TODO
        return 0;
    }

    std::vector<std::string> PasswordManager_SQLite3::getDomainLists()
    {
        // TODO
        return {};
    }

    void PasswordManager_SQLite3::errorLogCallback(void *pArg, int iErrCode, const char *zMsg)
    {
        (void)pArg;
        std::cerr << "(" << iErrCode << ") " << zMsg << std::endl;
    }
}

/* vim: set et: */
