/*
 * randomTool.cpp
 * Created on 2017/02/05 12:17:38
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "randomTool.h"
#include <random>

namespace RandomTool
{
    double zoURand()
    {
        static std::random_device rnd;
        static std::mt19937 mt(rnd());
        static std::uniform_real_distribution<> randzou(0,1);
        return randzou(mt);
    }
}
