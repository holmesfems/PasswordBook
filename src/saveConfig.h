/*
 * saveConfig.h
 * Created on 2017/03/25 20:23:49
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef _SAVECONFIG_H
#define _SAVECONFIG_H
#include <vector>
#include <string>
namespace SaveConfig
{
    class ConfigItem
    {
        public:
            ConfigItem();
            ConfigItem(const std::string& key,std::string value);
            std::string key;
            std::string value;
    };

    class Config
    {
        public:
            Config(){}
            int pushItem(ConfigItem item);
            int load(const std::string& filename);
            int save(const std::string& filename);
            std::string getValueByKey(const std::string& key);
            std::string getValueByIndex(int index);
            std::string getKeyByIndex(int index);
            int getIndexByKey(const std::string& key);
            int setValueByKey(const std::string& value,const std::string& key);
            int setValueByIndex(const std::string& value,int index);
            //int deleteByIndex(int index);
        private:
            std::vector<ConfigItem> _configList;
    };
}

#endif // _SAVECONFIG_H
