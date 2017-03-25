/*
 * saveConfig.cpp
 * Created on 2017/03/25 20:52:48
 * Copyright (C) 2017 zhai <holmesfems@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "saveConfig.h"
#include "stringTool.h"
#include <fstream>
#include <iostream>
#include <vector>
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
        std::ifstream ifs;
        ifs.open(filename);
        if(!(ifs.is_open()))
        {
            std::cerr << "Config:Can't open file:" << filename << std::endl;
            return -1;
        }
        if(!(_configList.empty()))
        {
            _configList.clear();
            _configList.shrink_to_fit();
        }
        while(!(ifs.eof()))
        {
            std::string line;
            std::getline(ifs,line);
            line=StringTool::strTrim(line);
            if(line.c_str()[0]=='#') continue;
            size_t equal=line.find("=");
            if(equal == std::string::npos)
                continue;
            std::vector<std::string> split=StringTool::strSep(line,equal,1);
            _configList.push_back(ConfigItem(StringTool::strTrim(split[0]),
                        StringTool::strTrim(split[1])));
        }
        ifs.close();
        return 0;
    }

    int Config::save(const std::string& filename)
    {
        if(_configList.empty())
        {
            std::cerr << "Config:The config is empty" << std::endl;
            return -3;
        }
        std::ifstream ifs;
        ifs.open(filename);
        if(!(ifs.is_open()))
        {
            std::cerr << "Config:Can't open file:" << filename << std::endl;
            return -1;
        }
        std::string swapFile=filename;
        swapFile+=".swp";
        std::ofstream ofs;
        ofs.open(swapFile,std::ofstream::trunc);
        if(!(ifs.is_open()))
        {
            std::cerr << "Config:Can't create file:" << swapFile << std::endl;
            return -2;
        }
        int totalWrite=_configList.size();
        char written[totalWrite];
        int i;
        for(i=0;i<totalWrite;i++)
            written=0;
        while(!(ifs.eof()))
        {
            std::string line;
            std::getline(ifs,line);
            if(line == "" && ifs.eof())
                break;
            std::string trimline=StringTool::strTrim(line);
            if(trimline.c_str()[0]=='#')
            {
                ofs << line << std::endl;
                continue;
            }
            size_t equal=line.find("=");
            if(equal == std::string::npos)
            {
                ofs << line << std::endl;
                continue;
            }
            std::vector<std::string> split=StringTool::strSep(trimline,equal,1);
            std::string key=StringTool::strTrim(split[0]);
            int index;
            if((index=getIndexByKey)<0)
            {
                ofs << line << std::endl;
            }
            else
            {
                if(written[index]==1)
                    std::cerr << "Config:Warning,the key:" << getKeyByIndex(index) << " has already be written" << std::endl;
                ofs << getKeyByIndex(index) << "\t=\t" << getValueByIndex(index) << std::endl;
                written[index]=1;
            }
        }
        for(i=0;i<totalWrite;i++)
        {
            if(!(written[i]))
                ofs << getKeyByIndex(i) << "\t=\t" << getValueByIndex(i) << std::endl;
        }
    }

    std::string Config::getValueByKey(const std::string& key)
    {
        return getValueByIndex(getIndexByKey(key));
    }

    std::string Config::getValueByIndex(int index)
    {
        return _configList[index].value;
    }

    std::string getKeyByIndex(int index)
    {
        return _configList[index].key;
    }

    int getIndexByKey(const std::string& key)
    {
        int i,maxi=_configList.size();
        for(i=0;i<maxi;i++)
        {
            if(_configList[i].key==key)
                return i;
        }
        return -1;
    }

    int setValueByKey(const std::string& value,const std::string& key)
    {
        int ret;
        if((ret=setValueByIndex(value,getIndexByKey(key)))<0)
        {
            pushItem(ConfigItem(key,value));
        }
        return ret;
    }

    int setValueByIndex(const std::string& value,int index)
    {
        if(index < 0 || index >= _configList.size())
        {
            return -1;
        }
        else
        {
            _configList[index].value=value;
        }
        return 0;
    }

}
