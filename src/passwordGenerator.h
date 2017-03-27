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
    //! The class to generate a randomized password
    class PasswordGenerator
    {
      public:
        //! Constructor
        PasswordGenerator();

        //! Destructor
        ~PasswordGenerator(){};

        //! Set the value of pwdList
        /*!
         * \return 0 if done normally
         */
        int setPwdList(const std::string& pwdList);

        //! Push a string to pwdList
        /*!
         * \return 0 if done normally
         */
        int pushPwdList(const std::string& pwdList);

        //! Clear the pwdList to empty
        /*!
         * \return 0 if done normally
         */
        int clearPwdList();

        //! Get the value of pwdList
        inline std::string getPwdList() { return _pwdList; }
        //! Set the value of pwdLength
        int setPwdLength(int pwdLength);

        //! Get the value of pwdLength
        inline int getPwdLength() { return _pwdLength; }
        //! Generate the randomized password
        /*!
         * The result is relied to pwdLength and pwdList. The length of result
         * will equal to pwdLength, and each charactor of the result will be
         * chosen randomly from pwdList.
         * \return A new string which is the result
         */
        std::string generate();

        //! Save the config to a certain file
        /*!
         * The config contain pwdLength and pwdList
         */
        int save(const std::string& filename);

        //! Load the config from a certain file
        /*!
         * \sa save()
         */
        int load(const std::string& filename);
        // std::string generateByExpr(const std::string& expr);

      private:
        //! The total length of the password which will be generated
        /*!
         * \sa generate()
         */
        int _pwdLength;

        //! The character list of the generation
        /*!
         * \sa generate()
         */
        std::string _pwdList;
    };

    //! A to Z
    extern const std::string bigAlphabet;

    //! a to z
    extern const std::string smallAlphabet;

    //! 0 to 9
    extern const std::string numeric;

    //! Other signs
    extern const std::string signs;
}

#endif  // _PASSWORDGENERATOR_H
