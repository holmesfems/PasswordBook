/*
 * saveConfig.cpp
 * Created on 2017/03/25 20:52:48
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "saveConfig.h"

namespace SaveConfig
{
    ConfigItem::ConfigItem()
    {
        key="";
        value="";
    }

    ConfigItem::ConfigItem(const std::string& key,const std::string& value)
    {
        this->key=key;
        this->value=value;
    }

    int Config::pushItem(ConfigItem item)
    {
        _configList.push_back(item);
        return 0;
    }

    int Config::load(const std::string& filename)
    {

    }

}
