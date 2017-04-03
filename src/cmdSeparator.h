/*
 * cmdSeparator.h
 * Created on 2017/03/25 18:30:51
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef _CMDSEPRATOR_H
#define _CMDSEPRATOR_H
#include <algorithm>
#include <boost/filesystem.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "miscUtil.h"
#include "passwordGenerator.h"
#include "passwordManager.h"
namespace CmdSeparator
{
    using Params = std::vector<std::string>;
    using CmdHandler = std::function<int(Params &)>;
    using CmdHelper = std::pair<CmdHandler, std::string>;
    using CmdMap = std::map<std::string, CmdHelper>;
    //! The class to execute the command which is sent by user
    /*!
     * While running the main application,this class should be constructed first,
     * and all commands will be sent to other modules via this class
     */
    class CmdSeparator
    {
      public:
        //! Constructor,doing some initialize operation
        CmdSeparator();

        //! Destructor,do this while exiting application
        ~CmdSeparator();

        //! Executing a certain command
        /*!
         * \return 0 if should exitting application, >0 if done normally, <0 if
         *   some error occured
         */
        int exec(const std::string &cmd);

        //! Register command with name 'cmd'
        /*!
         *  corresponding function is like:
         *
         *  ```
         *  int _cmd_func(Params &split_param) {...}
         *  ```
         */
        inline void registerCmd(std::string cmd, std::function<int(Params &)>,
                                std::string help_string = "");
        inline void registerCmd(std::string cmd, int (CmdSeparator::*f)(Params &),
                                std::string help_string = "");

      protected:
        //! dispatch cmd to corresponding handler
        int dispatchCmd(const std::string &cmd, Params &split_param);
        void initCmdDict();

      private:
        // std::string _thisPath;
        //! PasswordGenerator module
        PasswordGenerator::PasswordGenerator *_pwdGenerator;

        //! "dbname" -> PasswordManager
        std::map<std::string, std::unique_ptr<PasswordManager::PasswordManager> > _pManager;

        //! The output stream to show all messages
        std::ostream *_os;

        //! dictionary of cmd param to cmd function handler
        CmdMap _cmdmap;

        int _cmd_generate(Params &param);
        int _cmd_exit(Params &param);
        int _cmd_help(Params &param);
        int _cmd_opendb(Params &param);
        int _cmd_save_password_for_domain(Params &param);
        int _cmd_load_password(Params &param);
    };

    //! The default path of the config file of PasswordGenerator
    extern const boost::filesystem::path generatorConfigFile;
}

#endif  // _CMDSEPRATOR_H

/* vim: set et: */
