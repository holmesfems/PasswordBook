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
    const std::string VERSION = "0.0.0";
    const std::string TABLENAME = "pbook";
    typedef struct columnItem {
        std::string name;
        std::string type;
        std::string init;
    } columnItem;

    const std::vector<columnItem> COLUMN{{"id", "INTEGER", "PRIMARY KEY AUTOINCREMENT"},
                                         {"domain", "TEXT", "UNIQUE ON CONFLICT REPLACE"},
                                         {"pwdoutput", "BLOB", "NOT NULL"},
                                         {"encver", "TEXT", "NOT NULL"}};
    class PasswordManager
    {
      public:
        //~PasswordManager();
        //! Open the database file which save the password
        /*!
         * \param filename The name of db file
         * \return 0 for nomally, <0 for some error
         */
        virtual int32_t openDB(const std::string &filename) = 0;
        //! Show the index of domain by string contains
        /*!
         * \param domain The string to search
         * \return 0 for no error
         */
        virtual int32_t showIndexByDomain(const std::string &domain) = 0;
        //! Search the item by index
        /*!
         * \param id Index to search
         * \return The encrypted password data
         */
        virtual bytes searchByIndex(int32_t id) = 0;
        //! Add or replace an item by password and domain
        /*!
         * \param passwd The encrypted password bytes,not the plain text
         * \param domain The domain which associated to the password
         * \return 0 if no error occured
         */
        virtual int32_t addPasswd(bytes passwd, const std::string &domain) = 0;
        //! Add search the password by domain
        /*!
         * \param domain The string to search in domain lists
         * \return The encrypted password of the first item hit by domain
         */
        virtual bytes searchByDomain(const std::string &domain) = 0;
        //! Delete an item by index
        /*!
         * \param id The index of item
         * \return 0 for no error occured
         */
        virtual int32_t deleteByIndex(int32_t id) = 0;
        //! Get a list that contains all item of database
        /*!
         * \return The list of domain string
         */
        virtual std::vector<std::string> getDomainList() = 0;
        //! Get the version of this module
        virtual std::string version() { return VERSION; }
    };

    PasswordManager *FactoryCreateManager(const std::string &dbtype = "sqlite3");

    //! Password manager using sqlite3 database module
    class PasswordManager_SQLite3 : public PasswordManager
    {
      public:
        //! Constructor
        PasswordManager_SQLite3();
        //! Set version of encryption tool
        /*!
         * \sa _encVer
         */
        inline void setEncVer(const std::string encVer) { _encVer = encVer; }
        //! Get version of encryption tool
        /*!
         * \sa _encVer
         */
        inline std::string getEncVer() { return _encVer; }
        int32_t openDB(const std::string &filename) override;
        /*!
         * \sa _idlist
         */
        int32_t showIndexByDomain(const std::string &domain) override;
        /*!
         * If you want to get encryption version of the item,call "getencver()"
         * function AFTER this to get that.
         * \sa _encVer
         */
        bytes searchByIndex(int32_t id) override;
        /*!
         * If you want to set the encryption version of the record,call "setencver()"
         * function BEFORE this to set that
         * \sa _encVer
         */
        int32_t addPasswd(bytes passwd, const std::string &domain) override;
        /*!
         * This function will call "showIndexByDomain()" function first to detect
         * the id,then call "searchByIndex()" function to return the result.
         * Please use "getencver()" function to get the encryption version after
         * running this
         */
        bytes searchByDomain(const std::string &domain) override;
        int32_t deleteByIndex(int32_t id) override;
        std::vector<std::string> getDomainList() override;

        static void errorLogCallback(void *pArg, int iErrCode, const char *zMsg);

      private:
        //! initialize database
        int32_t _initDB();
        // int32_t _checkDB();
        // char **_getQuery(const char *sqlRequest, int32_t *pnRow = NULL, int32_t
        // *qnRow = NULL);
        //! The main connection to sqlite3 database
        sqlite3 *_conn;
        //! The output stream to output all messages
        std::ostream *_os;
        //! The version of encryption tool
        /*!
         * Before using "addpasswd()" function, you should run "setencver()" function
         *to
         * determine the version of encryption tool,and it will be automatically
         *recorded
         * while running "addpasswd()" function.Similarly,after you run
         *"searchByIndex()"
         * function, this will automatically changed and you should run "getencver()"
         *function
         * to get the value of this.
         *
         * The version will determine which method of encryption will be used
         */
        std::string _encVer = "0.0.0";
        // The list of id hit by "showIndexByDomain"
        /*!
         * After calling "showIndexByDomain()",this value will be automatically
         * changed.
         */
        std::vector<int32_t> _idlist;
    };
}

#endif  // _PASSWORDMANAGER_H

/* vim: set et: */
