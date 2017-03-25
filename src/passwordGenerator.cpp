/*
 * passwordGenerator.cpp
 * Created on 2017/03/25 19:15:03
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "passwordGenerator.h"
#include <sstream>
#include <fstream>
namespace PasswordGenerator
{
    const std::string bigAlphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string smallAlphabet="abcdefghijklmnopqrstuvwxyz";
    const std::string numeric="0123456789";
    const std::string signs=R"(!@#$%^&*()_=+*-`{}[]\ |'";:/?.>,<)";
    std::string strConnect(const string& str1,const string& str2)
    {
        ostringstream oss;
        oss << str1 << str2;
        return oss.str();
    }

    PasswordGenerator::PasswordGenerator()
    {
        _pwdLength=12;
        _pwdList=strConnect(strConnect(bigAlphabet,smallAlphabet),numeric);
    }

    int PasswordGenerator::setPwdList(const string& pwdList)
    {
        _pwdList=pwdList;
        return 0;
    }

    int PasswordGenerator::pushPwdList(const string& pwdList)
    {
        _pwdList=strConnect(_pwdList,pwdList);
        return 0;
    }

    int PasswordGenerator::clearPwdList()
    {
        _pwdList="";
        return 0;
    }

    int PasswordGenerator::setPwdLength(int pwdLength)
    {
        _pwdLength=pwdLength;
        return 0;
    }

    std::string PasswordGenerator::generate()
    {
        int i;
        ostringstream oss;
        int selectLength=_pwdList.length();
        char *pwdListCstr=_pwdList.c_str();
        for(i=0;i<_pwdLength;i++)
        {
            int locale=int(RandomTool::zoURand()*selectLength);
            oss << pwdListCstr[locale];
        }
        return oss.str();
    }

    int PasswordGenerator::saveToFile(const std::string& filename)
    {
        ofstream ofs;
        ofs.open(filename,std::ofstream::trunc);
        if(!ofs.is_open())
        {
            std::cerr << "Can't create file:" << filename << std::endl;
            return -1;
        }
        ofs << "pwdLength=" << _pwdLength << std::endl;
        ofs << R"("pwdList=\")" << _pwdList << R"(\")" << std::endl;
        ofs.close();
        return 0;
    }

    int PasswordGenerator::loadFromFile(const std::string& filename)
    {
        ifstream ifs;
        ifs.open(filename);
        if(!ifs.is_open())
        {
            std::cerr << "Can't open file:" << filename << std::endl;
            return -1;
        }
        string cmd;
        while(!ifs.eof())
        {
            std::getline(ifs,cmd);

        }
    }

}
