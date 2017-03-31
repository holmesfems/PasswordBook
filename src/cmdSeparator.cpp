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

        initCmdDict();
    }

    void CmdSeparator::initCmdDict()
    {
        registerCmd("generate", &CmdSeparator::_cmd_generate, "generate random string");
        registerCmd("exit", &CmdSeparator::_cmd_exit, "save and exit");
        registerCmd("help", &CmdSeparator::_cmd_help, "show this help");
    }

    CmdSeparator::~CmdSeparator() { _pwdGenerator->save(generatorConfigFile.string()); }
    int CmdSeparator::exec(const std::string &cmd)
    {
        std::string tcmd = StringTool::strTrim(cmd);
        if (tcmd == "") {
            (*_os) << "blank!" << std::endl;
            return 1;
        }

        Params split = StringTool::strSplit(tcmd, space);
        return dispatchCmd(split[0], split);
    }

    inline void CmdSeparator::registerCmd(std::string cmd, std::function<int(Params &)> handler,
                                          std::string help_string)
    {
        if (help_string.empty()) {
            help_string = typeid(handler).name();
        }
        _cmdmap[std::move(cmd)] = std::make_pair(handler, std::move(help_string));
    }

    inline void CmdSeparator::registerCmd(std::string cmd, int (CmdSeparator::*f)(Params &),
                                          std::string help_string)
    {
        registerCmd(std::move(cmd), std::bind(f, this, std::placeholders::_1),
                    std::move(help_string));
    }

    int CmdSeparator::dispatchCmd(const std::string &cmd, Params &split_param)
    {
        auto handler = _cmdmap[cmd];
        if (std::get<0>(handler)) {
            return std::get<0>(handler)(split_param);
        } else {
            (*_os) << "unknown command" << std::endl;
            return -1;
        }
    }

    int CmdSeparator::_cmd_generate(Params &param)
    {
        (void)param;
        (*_os) << _pwdGenerator->generate() << std::endl;
        return 2;
    }

    int CmdSeparator::_cmd_exit(Params &param)
    {
        (void)param;
        (*_os) << "exit!" << std::endl;
        return 0;
    }

    int CmdSeparator::_cmd_help(Params &param)
    {
        (void)param;
        for (auto &c : _cmdmap) {
            std::string p;
            std::tie(std::ignore, p) = c.second;
            (*_os) << "\t" << c.first << ": " << p << std::endl;
        }
        return 2;
    }
}

/* vim: set et: */
