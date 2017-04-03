/*
 * cmdSeparator.cpp
 * Created on 2017/03/26 11:47:18
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */
#include "cmdSeparator.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include "crypto.h"
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
        registerCmd("opendb", &CmdSeparator::_cmd_opendb,
                    "open passwd manager db (opendb <dbname>)");
        registerCmd("addpasswd", &CmdSeparator::_cmd_save_password_for_domain,
                    "record password into db (addpasswd <dbname> <domain>)");
        registerCmd("exit", &CmdSeparator::_cmd_exit, "save and exit");
        registerCmd("help", &CmdSeparator::_cmd_help, "show this help");
        registerCmd("loadpass", &CmdSeparator::_cmd_load_password,
                    "load password (loadpass <dbname> <domain>");
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
#ifdef DEBUG
        std::for_each(split.begin(), split.end(), [](std::string &v) { std::cout << v << ", "; });
        std::cout << std::endl;
#endif
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

    int CmdSeparator::_cmd_opendb(Params &param)
    {
        if (param.size() <= 1) {
            (*_os) << "error: need param <dbname>" << std::endl;
            return 2;
        }
        auto &pm = _pManager[param[1]] = std::unique_ptr<PasswordManager::PasswordManager>(
            PasswordManager::FactoryCreateManager());
        if (!pm) {
            (*_os) << "invalid db" << std::endl;
        }
        int status = pm->openDB(param[1]);
        if (status) {
            (*_os) << "failed" << std::endl;
            return 2;
        }
        (*_os) << "db " << param[1] << " opened" << std::endl;
        return 2;
    }

    int CmdSeparator::_cmd_save_password_for_domain(Params &param)
    {
        // required param: <dbname> <domain>
        // additonal input: <pwd0> <password>
        if (param.size() <= 2) {
            //(*_os) << "error: need param <dbname> <domain> <pwd0> <pwdoutput>" <<
            // std::endl;
            (*_os) << "error: need param <dbname> <domain>" << std::endl;
            return 2;
        }
        std::string dbname = std::move(param[1]);
        std::string domain = std::move(param[2]);
        auto &pm = _pManager[dbname];
        if (!pm) {
            (*_os) << "error: db " << dbname << " not opened" << std::endl;
            return 2;
        }

        std::cout << "input your master key: " << std::flush;
        std::string pwd0 = ReadPassword<std::string>();
        std::cout << "input your password for domain: " << domain << std::flush;
        std::string pwdoutput = ReadPassword<std::string>();
        auto encrypted_passwd = Crypto::encrypt(pwd0, pwdoutput);

        int status = pm->addPasswd(std::move(encrypted_passwd), domain);
        if (status) {
            (*_os) << "error: add failed" << std::endl;
        } else {
            (*_os) << "save success" << std::endl;
        }
        return 2;
    }

    int CmdSeparator::_cmd_load_password(Params &param)
    {
        if (param.size() <= 2) {
            (*_os) << "error: need param <dbname> <domain>" << std::endl;
            return 2;
        }
        std::string dbname = param[1];
        std::string domain = param[2];
        auto &pm = _pManager[dbname];
        if (!pm) {
            (*_os) << "error: db " << dbname << " not opened" << std::endl;
            return 2;
        }

        std::cout << "input your master key: " << std::flush;
        std::string pwd0 = ReadPassword<std::string>();

        PasswordManager::bytes encrypted_passwd = pm->searchByDomain(domain);
        if (encrypted_passwd.empty()) {
            (*_os) << "error: domain " << domain << " not found" << std::endl;
            return 2;
        }
        std::string passwd = Crypto::decrypt(pwd0, encrypted_passwd);
        std::cout << "passwd: " << passwd << std::endl;
        return 2;
    }
}

/* vim: set et: */
