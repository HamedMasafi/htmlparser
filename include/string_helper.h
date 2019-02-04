#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <string>
#include <vector>

class string_helper
{
public:
    string_helper();
    static std::string trim_copy(std::string s);
    static std::string rtrim_copy(std::string s);
    static std::string ltrim_copy(std::string s);
    static void trim(std::string &s);
    static void rtrim(std::string &s);
    static void ltrim(std::string &s);

    static void tolower(std::string &str);
    static void toupper(std::string &str);
    static bool replace(std::string &str, const std::string &from, const std::string &to);
    static std::vector<std::string> split(std::string str, const char &sep);
};

#endif // STRING_HELPER_H
