#ifndef _STRINGTOOL_H
#define _STRINGTOOL_H
#include<vector>
#include<string>
namespace StringTool
{
    std::vector<std::string> strSplit(const std::string& s,const std::string& delim);
    std::vector<std::string> strSep(const std::string& str,size_t locate,size_t length);
    std::string strTrim(const std::string& str,const std::string trim="\n\t ");
    template <typename T>
        std::string convertFrom(T from);
    template <typename T>
        T convertTo(const std::string& str);
}
#endif
