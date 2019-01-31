#include "string_helper.h"

#include <corecrt_wctype.h>
#include <algorithm>
#include <wctype.h>

using namespace std;

string_helper::string_helper()
{

}

void string_helper::ltrim(wstring &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](wint_t ch) {
        return !iswspace(ch);
    }));
}

void string_helper::toupper(wstring &str)
{
    transform(
      str.begin(), str.end(),
      str.begin(),
      towupper);
}
void string_helper::tolower(wstring &str)
{
    transform(
      str.begin(), str.end(),
      str.begin(),
      towlower);
}

// trim from end (in place)
void string_helper::rtrim(wstring &s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](wint_t ch) {
        return !iswspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void string_helper::trim(wstring &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
wstring string_helper::ltrim_copy(wstring s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
wstring string_helper::rtrim_copy(wstring s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
wstring string_helper::trim_copy(wstring s) {
    trim(s);
    return s;
}


bool string_helper::replace(wstring& str, const wstring& from, const wstring& to) {
    size_t start_pos = str.find(from);
    if(start_pos == wstring::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
