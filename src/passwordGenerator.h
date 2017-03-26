/*
 * passwordGenerator.h
 * Created on 2017/03/25 12:21:59
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef _PASSWORDGENERATOR_H
#define _PASSWORDGENERATOR_H
#include <string>
#include "randomTool.h"

namespace PasswordGenerator
{
    class PasswordGenerator
    {
        public:
            PasswordGenerator();
            ~PasswordGenerator(){};
            int setPwdList(const string& pwdList);
            int pushPwdList(const string& pwdList);
            int clearPwdList();
            inline std::string getPwdList(){return _pwdList;}
            int setPwdLength(int pwdLength);
            inline int getPwdLength(){return _pwdLength;}
            std::string generate();
            int save(const std::string& filename);
            int load(const std::string& filename);
            //std::string generateByExpr(const std::string& expr);

        private:
            int _pwdLength;
            std::string _pwdList;

    };
    extern const std::string bigAlphabet;
    extern const std::string smallAlphabet;
    extern const std::string numeric;
    extern const std::string signs;

}

#endif // _PASSWORDGENERATOR_H
