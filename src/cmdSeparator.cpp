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
#include "stdio.h"
#include "stdlib.h"
#include "stringTool.h"
namespace CmdSeparator
{
    const boost::filesystem::path generatorConfigFile("generator.conf");
    const boost::filesystem::path DBPath(".pbook.db", "~/");
    const boost::filesystem::path GlobalConfigFile(".pbook.conf", "~/");
    const std::string space = " ";
    void CmdSeparateHelper::bind(const std::string &key, void *target, int8_t type)
    {
        VariableBindItem vbi(target, type);
        _vb.insert(std::pair<std::string, VariableBindItem>(key, vbi));
    }

    void CmdSeparateHelper::bind(const char *key, void *target, int8_t type)
    {
        std::string keystr = key;
        bind(keystr, target, type);
    }

    void CmdSeparateHelper::set(Params &params)
    {
        for (auto item : params) {
            VariableBind::iterator iter;
            if ((iter = _vb.find(item.first)) != _vb.end()) {
                VariableBindItem vbi = iter->second;
                switch (vbi.second) {
                    case INTEGER:
                        *((int32_t *)(vbi.first)) = StringTool::convertTo<int32_t>(item.second);
                        break;
                    case TEXT:
                        *((std::string *)(vbi.first)) = item.second;
                        break;
                }
            }
        }
    }
    CmdSeparator::CmdSeparator()
    {
        initPG(generatorConfigFile);
        initPMDB(DBPath);
        initCmdDict();
    }
    CmdSeparator::CmdSeparator(const std::string &dbpath)
    {
        initPG(generatorConfigFile);
        initPMDB(boost::filesystem::path(dbpath));
        initCmdDict();
    }

    void CmdSeparator::initPMDB(const boost::filesystem::path &dbpath)
    {
        _pwdManager = PasswordManager::FactoryCreateManager("sqlite3");
        _pwdManager->openDB(dbpath.string());
    }

    void CmdSeparator::initPG(const boost::filesystem::path &config)
    {
        _pwdGenerator = new PasswordGenerator::PasswordGenerator;
        boost::system::error_code error;
        if (boost::filesystem::exists(config, error)) {
            _pwdGenerator->load(config.string());
        }
    }

    void CmdSeparator::initCmdDict()
    {
        registCmd("gen", &CmdSeparator::_cmd_generate, "generate random string");
        // registCmd("opendb", &CmdSeparator::_cmd_opendb,
        //            "open passwd manager db (opendb <dbname>)");
        registCmd("add", &CmdSeparator::_cmd_save_password_for_domain,
                  "record password into db (addpasswd <domain>)");
        registCmd("exit", &CmdSeparator::_cmd_exit, "save and exit");
        registCmd("help", &CmdSeparator::_cmd_help, "show this help");
        registCmd("load", &CmdSeparator::_cmd_load_password, "load password (loadpass <domain>");
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

        CmdSend tosend = _str_to_cmd(tcmd);
#ifdef DEBUG
        std::for_each(split.begin(), split.end(), [](std::string &v) { std::cout << v << ", "; });
        std::cout << std::endl;
#endif
        return dispatchCmd(tosend.first, tosend.second);
    }

    inline void CmdSeparator::registCmd(std::string cmd, std::function<int(Params &)> handler,
                                        std::string help_string)
    {
        if (help_string.empty()) {
            help_string = typeid(handler).name();
        }
        _cmdmap[std::move(cmd)] = std::make_pair(handler, std::move(help_string));
    }

    inline void CmdSeparator::registCmd(std::string cmd, int (CmdSeparator::*f)(Params &),
                                        std::string help_string)
    {
        registCmd(std::move(cmd), std::bind(f, this, std::placeholders::_1),
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
        return 3;
    }
    /*
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
    }*/

    int CmdSeparator::_cmd_save_password_for_domain(Params &param)
    {
        // required param: <dbname> <domain>
        // additonal input: <pwd0> <password>
        if (param.size() <= 1) {
            //(*_os) << "error: need param <dbname> <domain> <pwd0> <pwdoutput>" <<
            // std::endl;
            (*_os) << "error: need param <domain>" << std::endl;
            return -2;
        }
        CmdSeparateHelper csh;
        std::string domain;
        csh.bind("domain", (void *)(&domain), CmdSeparateHelper::TEXT);
        csh.bind("", (void *)(&domain), CmdSeparateHelper::TEXT);
        csh.set(param);
        while (domain == "") {
            std::cout << "input the domain:";
            std::cin >> domain;
        }
        std::string pwd0, pwd0c;
        std::cout << "input your master key: " << std::flush;
        pwd0 = ReadPassword<std::string>();
        std::cout << "confirm your master key: " << std::flush;
        pwd0c = ReadPassword<std::string>();
        if (pwd0 != pwd0c) {
            (*_os) << "Confirm again!" << std::endl;
            return -2;
        }
        std::cout << "input your password for domain: " << domain << std::flush;
        std::string pwdoutput = ReadPassword<std::string>();
        auto encrypted_passwd = Crypto::encrypt(pwd0, pwdoutput);
        int status = _pwdManager->addPasswd(std::move(encrypted_passwd), domain);
        if (status) {
            (*_os) << "error: add failed" << std::endl;
        } else {
            (*_os) << "saved successfully" << std::endl;
        }
        return 2;
    }

    int CmdSeparator::_cmd_load_password(Params &param)
    {
        std::string domain;
        int32_t index = -1;
        CmdSeparateHelper csh;
        csh.bind("", &domain, CmdSeparateHelper::TEXT);
        csh.bind("domain", &domain, CmdSeparateHelper::TEXT);
        csh.bind("index", &index, CmdSeparateHelper::INTEGER);
        csh.set(param);
        if (domain == "" && index < 0) {
            (*_os) << "Need param <domain> or <index>" << std::endl;
            return -2;
        }
        std::cout << "input your master key: " << std::flush;
        std::string pwd0 = ReadPassword<std::string>();
        PasswordManager::bytes encrypted_passwd;
        if (index >= 0)
            encrypted_passwd = _pwdManager->searchByIndex(index);
        else
            encrypted_passwd = _pwdManager->searchByDomain(domain);
        if (encrypted_passwd.empty()) {
            (*_os) << "error: domain " << domain << " not found" << std::endl;
            return -2;
        }
        std::string passwd = Crypto::decrypt(pwd0, encrypted_passwd);
        std::cout << "passwd: " << passwd << std::endl;
        return 2;
    }

    CmdSend CmdSeparator::_str_to_cmd(const std::string &cmdstr)
    {
        std::string trimCmd = StringTool::strTrim(cmdstr);
        bool first = true;
        // bool inStr=false;
        // bool inEsc=false;
        // bool inSpace=false;
        // bool needEnd=true;
        bool inEqual = false;
        const char *data = trimCmd.c_str();
        size_t i, maxi = trimCmd.length();
        std::ostringstream oss;
        std::string key, value;
        std::string cmd;
        Params ret;
        for (i = 0; i < maxi; i++) {
            if (data[i] == _escape) {
                i += 1;
                assert(i >= maxi);
                char *hexcode;
                std::ostringstream oss2;
                switch (data[i]) {
                    case 'v':
                        oss << '\v';
                        break;
                    case 'n':
                        oss << '\n';
                        break;
                    case 't':
                        oss << '\t';
                        break;
                    case 'r':
                        oss << '\r';
                        break;
                    case 'x':
                        assert(i + 2 >= maxi);
                        oss2 << data[i + 1] << data[i + 2];
                        hexcode = StringTool::strToBin(oss2.str());
                        oss << hexcode[0];
                        free(hexcode);
                        i += 2;
                        break;
                    default:
                        oss << data[i];
                }
                // inEsc = false;
                continue;
            }
            if (data[i] == _devide) {
                // if(inSpace) continue;
                do {
                    i++;
                } while (i < maxi && data[i] == _devide);
                i -= 1;
                if (first) {
                    cmd = oss.str();
                    oss.str("");
                    first = false;
                    continue;
                }
                value = oss.str();
                ParamItem item(key, value);
                ret.push_back(item);
                key = "";
                value = "";
                oss.str("");
                // inSpace = true;
                continue;
            }
            if (data[i] == _equal) {
                assert(first);
                assert(i == maxi - 1);
                if (!inEqual) {
                    key = oss.str();
                    oss.str("");
                    inEqual = true;
                } else {
                    oss << data[i];
                }
                continue;
            }
            oss << data[i];
            // TODO
        }  // for
        if (value != "" && oss.str() != "") {
            value = oss.str();
            ParamItem item(key, value);
            ret.push_back(item);
        }
        return CmdSend(cmd, ret);
    }
}

/* vim: set et: */
