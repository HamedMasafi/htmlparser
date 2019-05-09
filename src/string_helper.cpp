#include "string_helper.h"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <wctype.h>

PARSER_BEGIN_NAMESPACE

string_helper::string_helper()
{

}

void string_helper::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void string_helper::toupper(std::string &str)
{
    transform(
      str.begin(), str.end(),
      str.begin(),
      towupper);
}
void string_helper::tolower(std::string &str)
{
    transform(
      str.begin(), str.end(),
      str.begin(),
      towlower);
}

// trim from end (in place)
void string_helper::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void string_helper::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
std::string string_helper::ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
std::string string_helper::rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
std::string string_helper::trim_copy(std::string s) {
    trim(s);
    return s;
}


bool string_helper::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::vector<std::string> string_helper::split(std::string str, const char &sep)
{
    std::string temp;
    std::vector<std::string> parts;
    std::stringstream wss(str);
    while(std::getline(wss, temp, sep))
        parts.push_back(temp);
    return parts;
}

PARSER_END_NAMESPACE
