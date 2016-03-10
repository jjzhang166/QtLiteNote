#pragma once

#include <string>
namespace ln
{
    std::string UTF16ToUTF8(const std::wstring &ws);

    std::string UTF16ToGBK(const std::wstring &ws);
    std::wstring UTF8ToUTF16(const std::string &s);
    std::wstring GBKToUTF16(const std::string &s);
    std::string UTF8ToGBK(const std::string &utf8);
    std::string GBKToUTF8(const std::string &gbk);
}