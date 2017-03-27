/*
 * cmdSeparator.cpp
 * Created on 2017/03/26 11:47:18
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */
#include "cmdSeparator.h"
#include <iostream>
#include <vector>
#include "stringTool.h"
namespace CmdSeparator
{
    const boost::filesystem::path generatorConfigFile("generator.conf");
    const std::string space = " ";
    CmdSeparator::CmdSeparator()
    {
        _pwdGenerator = new PasswordGenerator::PasswordGenerator;
        boost::system::error_code error;
        if (boost::filesystem::exists(generatorConfigFile, error)) {
            _pwdGenerator->load(generatorConfigFile.string());
        }
        _os = &(std::cout);
    }

    CmdSeparator::~CmdSeparator() { _pwdGenerator->save(generatorConfigFile.string()); }
    int CmdSeparator::exec(const std::string &cmd)
    {
        std::string tcmd = StringTool::strTrim(cmd);
        if (tcmd == "") {
            (*_os) << "blank!" << std::endl;
            return 1;
        }
        std::vector<std::string> split = StringTool::strSplit(tcmd, space);
        if (split[0] == "exit") {
            (*_os) << "exit!" << std::endl;
            return 0;
        }
        if (split[0] == "generate") {
            (*_os) << _pwdGenerator->generate() << std::endl;
            return 2;
        }
        (*_os) << "unknown command" << std::endl;
        return -1;
    }
}
