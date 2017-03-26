#ifndef _STRINGTOOL_H
#define _STRINGTOOL_H
#include <vector>
#include <string>
namespace StringTool
{
    std::vector<std::string> strSplit(const std::string& s,const std::string& delim);
    std::vector<std::string> strSep(const std::string& str,size_t locate,size_t length);
    std::string strTrim(const std::string& str,const std::string trim="\n\t ");
    template <class T>
        std::string convertFrom(T from);
    template <class T>
        T convertTo(const std::string& str);

    std::string binToStr(const char* binary,int length);
    const char* strToBin(const std::string& str,int *length=NULL);
    std::string bracket(const std::string& str);
}
#include "stringTool_util.h"
#endif
