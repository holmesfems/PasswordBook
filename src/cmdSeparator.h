/*
 * cmdSeparator.h
 * Created on 2017/03/25 18:30:51
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef _CMDSEPRATOR_H
#define _CMDSEPRATOR_H
#include <string>
#include "passwordGenerator.h"
#include <boost/filesystem.hpp>
namespace CmdSeparator
{
    class CmdSeparator
    {
        public:
            CmdSeparator();
            ~CmdSeparator();
            int exec(const std::string& cmd);
        private:
            //std::string _thisPath;
            PasswordGenerator::PasswordGenerator *_pwdGenerator;
            std::ostream *_os;
    };

    extern const boost::filesystem::path generatorConfigFile;
}

#endif // _CMDSEPRATOR_H
