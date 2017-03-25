/*
 * passwordGenerator.h
 * Created on 2017/03/25 12:21:59
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef _PASSWORDGENERATOR_H
#define _PASSWORDGENERATOR_H
#include<string>

namespace PasswordGenerator
{
    class PasswordGenerator
    {
        public:
            PasswordGenerator();
            ~PasswordGenerator();
            int setPwdList(const string& pwdList);
            int addPwdList(const string& pwdList);
            int setPwdLength(int pwdLength);
            std::string generate();

        private:
            int _pwdLength;
            std::string _pwdList;
            
    };
    extern const string bigAlphabet;
    extern const string smallAlphabet;
    extern const string numeric;
    extern const string signs;

}

#endif // _PASSWORDGENERATOR_H
