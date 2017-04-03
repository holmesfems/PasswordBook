#ifndef _REGEXTOOL
#define _REGEXTOOL
#include <regex>
#include <string>
#include <vector>

std::vector<std::smatch *> regex_searchAll(const std::regex &regex,
                                           const std::string &target);
std::vector<std::smatch *> regex_searchOne(const std::regex &regex,
                                           const std::string &target);

void clearSmatch(std::vector<std::smatch *> &vec);
#endif //_REGEXTOOL
