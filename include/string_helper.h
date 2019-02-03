#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <string>
#include <vector>

class string_helper
{
public:
    string_helper();
    static std::wstring trim_copy(std::wstring s);
    static std::wstring rtrim_copy(std::wstring s);
    static std::wstring ltrim_copy(std::wstring s);
    static void trim(std::wstring &s);
    static void rtrim(std::wstring &s);
    static void ltrim(std::wstring &s);

    static void tolower(std::wstring &str);
    static void toupper(std::wstring &str);
    static bool replace(std::wstring &str, const std::wstring &from, const std::wstring &to);
    static std::vector<std::wstring> split(std::wstring str, const wint_t &sep);
};

#endif // STRING_HELPER_H
