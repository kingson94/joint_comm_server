/*
* File: Utils.h
* Class: Utils
* Created: 20211107
* Author: SonTV
*/
#pragma once
#include <string>
#include "Define.h"
#include <cstdarg>
#include "cppcms/json.h"
#include <fstream>
#include <sstream>
namespace util
{
class Utils;
}

#ifndef MAX_LOG_SIZE
#define MAX_LOG_SIZE 2048
#endif // MAX_LOG_SIZE

#ifndef LOG
#define LOG(x) \
    { \
        std::string strTmp(__FILE__); \
        std::string strRelativeFilePath = strTmp; \
        size_t iLastSlashPos = strTmp.rfind('/'); \
        if (iLastSlashPos != std::string::npos) \
        { \
            strRelativeFilePath = strTmp.substr(iLastSlashPos + 1); \
        } \
        std::printf("%s:%d] %s\n", strRelativeFilePath.c_str(), __LINE__, x); \
    } \

#endif // LOG

#ifndef LOG2
#define LOG2(x, ...) \
    { \
        std::string strTmp(__FILE__); \
        std::string strRelativeFilePath = strTmp; \
        std::string strDataFilled = ""; \
        size_t iLastSlashPos = strTmp.rfind('/'); \
        util::Utils::LogBuilder(strDataFilled, x, ##__VA_ARGS__); \
        if (iLastSlashPos != std::string::npos) \
        { \
            strRelativeFilePath = strTmp.substr(iLastSlashPos + 1); \
        } \
        std::printf("%s:%d] %s\n", strRelativeFilePath.c_str(), __LINE__, strDataFilled.c_str()); \
    } \

#endif // LOG2

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

    static inline int LoadJsonFromFile(const std::string &strFile, cppcms::json::value &jValue)
    {
        std::ifstream ifStream(strFile.c_str());
        if (!jValue.load(ifStream, true))
        {
            ifStream.close();
            return -1;
        }
        
        ifStream.close();
        return 0;
    }

    static inline int StringToJson(const std::string &strData, cppcms::json::value &jValue)
    {
        std::istringstream issBuffer(strData);
        if (!jValue.load(issBuffer, true))
        {
            return -1;
        }

        return 0;
    }
};
} // namespace util
