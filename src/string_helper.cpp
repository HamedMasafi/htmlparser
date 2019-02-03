#include "string_helper.h"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <wctype.h>

string_helper::string_helper()
{

}

void string_helper::ltrim(std::wstring &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void string_helper::toupper(std::wstring &str)
{
    transform(
      str.begin(), str.end(),
      str.begin(),
      towupper);
}
void string_helper::tolower(std::wstring &str)
{
    transform(
      str.begin(), str.end(),
      str.begin(),
      towlower);
}

// trim from end (in place)
void string_helper::rtrim(std::wstring &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void string_helper::trim(std::wstring &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
std::wstring string_helper::ltrim_copy(std::wstring s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
std::wstring string_helper::rtrim_copy(std::wstring s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
std::wstring string_helper::trim_copy(std::wstring s) {
    trim(s);
    return s;
}


bool string_helper::replace(std::wstring& str, const std::wstring& from, const std::wstring& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::wstring::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::vector<std::wstring> string_helper::split(std::wstring str, const wint_t &sep)
{
    std::wstring temp;
    std::vector<std::wstring> parts;
    std::wstringstream wss(str);
    while(std::getline(wss, temp, L';'))
        parts.push_back(temp);
    return parts;
}
