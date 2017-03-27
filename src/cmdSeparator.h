/*
 * cmdSeparator.h
 * Created on 2017/03/25 18:30:51
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef _CMDSEPRATOR_H
#define _CMDSEPRATOR_H
#include <boost/filesystem.hpp>
#include <string>
#include "passwordGenerator.h"
namespace CmdSeparator
{
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

      private:
        // std::string _thisPath;
        //! PasswordGenerator module
        PasswordGenerator::PasswordGenerator *_pwdGenerator;

        //! The output stream to show all messages
        std::ostream *_os;
    };

    //! The default path of the config file of PasswordGenerator
    extern const boost::filesystem::path generatorConfigFile;
}

#endif  // _CMDSEPRATOR_H
