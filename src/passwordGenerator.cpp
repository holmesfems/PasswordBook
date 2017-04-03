/*
 * passwordGenerator.cpp
 * Created on 2017/03/25 19:15:03
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "passwordGenerator.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "regextool.h"
#include "saveConfig.h"
#include "stringTool.h"

namespace PasswordGenerator
{
    const std::string bigAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string smallAlphabet = "abcdefghijklmnopqrstuvwxyz";
    const std::string numeric = "0123456789";
    const std::string signs = R"(!@#$%^&*()_=+*-`{}[]\ |'";:/?.>,<)";
    std::string strConnect(const std::string &str1, const std::string &str2)
    {
        std::ostringstream oss;
        oss << str1 << str2;
        return oss.str();
    }

    PasswordGenerator::PasswordGenerator()
    {
        _pwdLength = 12;
        _pwdList = strConnect(strConnect(bigAlphabet, smallAlphabet), numeric);
    }

    int PasswordGenerator::setPwdList(const std::string &pwdList)
    {
        _pwdList = pwdList;
        return 0;
    }

    int PasswordGenerator::pushPwdList(const std::string &pwdList)
    {
        _pwdList = strConnect(_pwdList, pwdList);
        return 0;
    }

    int PasswordGenerator::clearPwdList()
    {
        _pwdList = "";
        return 0;
    }

    int PasswordGenerator::setPwdLength(int pwdLength)
    {
        _pwdLength = pwdLength;
        return 0;
    }

    std::string PasswordGenerator::generate()
    {
        int i;
        std::ostringstream oss;
        int selectLength = _pwdList.length();
        const char *pwdListCstr = _pwdList.c_str();
        for (i = 0; i < _pwdLength; i++) {
            int locale = int(RandomTool::zoURand() * selectLength);
            oss << pwdListCstr[locale];
        }
        return oss.str();
    }

    const std::string key1 = "pwdLength";
    const std::string key2 = "pwdList";

    int PasswordGenerator::save(const std::string &filename)
    {
        SaveConfig::Config config;
        std::string val1 = StringTool::convertFrom<int>(_pwdLength);
        std::string val2 = _pwdList;
        config.setValueByKey(val1, key1);
        config.setValueByKey(StringTool::bracket(val2), key2);
        return config.save(filename);
    }

    int PasswordGenerator::load(const std::string &filename)
    {
        SaveConfig::Config config;
        int ret;
        if ((ret = config.load(filename)) != 0) {
            std::cerr << "Error occured while loading config file" << std::endl;
            return ret;
        }
        std::string val1 = config.getValueByKey(key1);
        if (val1 == "") {
            std::cerr << "No value of key:" << key1;
            return -1;
        }
        std::string val2 = config.getValueByKey(key2);
        if (val2 == "") {
            std::cerr << "No value of key:" << key2;
            return -1;
        }
        _pwdLength = StringTool::convertTo<int>(val1);
        std::regex listval(R"(".*")");
        std::vector<std::smatch *> listmatch = regex_searchOne(listval, val2);
        if (listmatch.size() == 0) {
            std::cerr << "Wrong type value:" << val2;
            return -2;
        }
        std::string rval2 = listmatch[0]->str();
        int rval2length = rval2.length();
        _pwdList = rval2.substr(1, rval2length - 2);
        clearSmatch(listmatch);
        return 0;
    }
}
