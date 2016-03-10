#include "utility.h"
#include <windows.h>

namespace ln
{
    std::string UTF16ToUTF8(const std::wstring &ws)
    {
        if (ws.empty()) {
            return std::string("");
        }
        std::string s = "";

        if (!ws.empty()) {
            int n = ::WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), ws.size(), NULL, 0, 0, 0);
            if (n > 0) {
                s.resize(n);
                ::WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), ws.size(), (char *)&s.at(0), n, 0, 0);
            }
        }

        return s;
    }

    std::string UTF16ToGBK(const std::wstring &ws)
    {
        if (ws.empty()) {
            return std::string("");
        }
        std::string s;
        int n = ::WideCharToMultiByte(936, 0, ws.c_str(), ws.size(), NULL, 0, 0, 0);
        if (n > 0) {
            s.resize(n);
            ::WideCharToMultiByte(936, 0, ws.c_str(), ws.size(), (char *)&s.at(0), n, 0, 0);
        }

        return s;
    }

    std::wstring UTF8ToUTF16(const std::string &s)
    {
        if (s.empty()) {
            return std::wstring(L"");
        }
        std::wstring ws = L"";

        if (!s.empty()) {
            int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), s.size(), NULL, 0);
            if (n > 0) {
                ws.resize(n);
                ::MultiByteToWideChar(CP_UTF8, 0, s.c_str(), s.size(), (wchar_t *)&ws.at(0), n);
            }
        }

        return ws;
    }

    std::wstring GBKToUTF16(const std::string &s)
    {
        if (s.empty()) {
            return std::wstring(L"");
        }
        std::wstring ws;
        int n = MultiByteToWideChar(936, 0, s.c_str(), s.size(), NULL, 0);

        if (n > 0) {
            ws.resize(n);
            ::MultiByteToWideChar(936, 0, s.c_str(), s.size(), (wchar_t *)&ws.at(0), n);
        }

        return ws;
    }

    std::string UTF8ToGBK(const std::string &utf8)
    {
        return UTF16ToGBK(UTF8ToUTF16(utf8));
    }

    std::string GBKToUTF8(const std::string &gbk)
    {
        return UTF16ToUTF8(GBKToUTF16(gbk));
    }

}
