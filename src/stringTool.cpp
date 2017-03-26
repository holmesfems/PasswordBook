#include "stringTool.h"
#include <sstream>
//#define DEBUG
namespace StringTool
{
    std::vector<std::string> strSplit(
            const std::string& s,
            const std::string& delim)
    {
    #ifdef DEBUG
        std::cout << "calling strsplit( "<<s<<" , "<<delim<<" )\n";
    #endif
        std::vector<std::string> result;
        std::string::size_type pos=0;
        while(pos!=std::string::npos)
        {
            std::string::size_type p = s.find(delim,pos);
            if(p==std::string::npos)
            {
                result.push_back(s.substr(pos));
                break;
            }
            else
            {
                result.push_back(s.substr(pos,p-pos));
            }
            pos=p+delim.size();
        }
        return result;
    }

    std::vector<std::string> strSep(const std::string &str,size_t locate,size_t length)
    {
    #ifdef DEBUG
        std::cout << "calling strSep(" << str << "," << locate << "," << length << ")" << std::endl;
    #endif
        std::vector<std::string> result;
        result.push_back(str.substr(0,locate));
        result.push_back(str.substr(locate+length,str.length()-locate-length));
    #ifdef DEBUG
        std::cout <<"result:" << result[0] << " | " << result[1] << std::endl;
    #endif
        return result;
    }

    std::string strTrim(const std::string& str,const std::string trim)
    {
        size_t start = str.find_first_not_of(trim);
        if(start == std::string::npos)
            return "";
        size_t last = str.find_last_not_of(trim);
        return str.substr(start,last-start+1);
    }

    template <typename T>
        std::string convertFrom(T from)
    {
        std::ostringstream oss;
        oss << from;
        return oss.str();
    }

    template <typename T>
        T convertTo(const std::string& str)
    {
        std::istringstream iss;
        iss.str(str);
        T ret;
        iss >> ret;
        return ret;
    }

    template convertFrom<int>;
    template convertFrom<double>;
    template convertTo<int>;
    template convertTo<double>;
}
