/*
* File: Utils.h
* Class: Utils
* Created: 20211107
* Author: SonTV
*/
#pragma once
#include <string>
#include "log/LogDefine.h"
#include <cstdarg>
#include "nlohmann/json.hpp"
#include <fstream>

namespace util
{
class Utils;
}


namespace util
{
class Utils
{
public:
    Utils();
    ~Utils();
    static inline void LogBuilder(std::string &strOut, const char *szFormat, ...)
    {
        char szBuffer[MAX_LOG_SIZE];
        va_list args;

        va_start(args, szFormat);
        vsnprintf(szBuffer, MAX_LOG_SIZE, szFormat, args);
        strOut = szBuffer;
        va_end(args);
    }

    static inline int LoadJsonFromFile(const std::string &strFile, nlohmann::json &jValue)
    {
        std::ifstream ifStream(strFile.c_str());
        ifStream >> jValue;
        if (jValue.empty())
        {
            return -1;
        }
        
        return 0;
    }

    static inline int StringToJson(const std::string &strData, nlohmann::json &jValue)
    {
        jValue = nlohmann::json::parse(strData);
        if (jValue.empty())
        {
            return -1;
        }
        
        return 0;
    }
};
} // namespace util
