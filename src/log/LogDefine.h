/*
* File: LogDefine.h
* Created: 20211127
* Author: SonTV
*/
#pragma once

#ifndef _LOG_MONITOR_
#define _LOG_MONITOR_
#endif // _LOG_MONITOR_

#ifndef _LOG_FILE_
#define _LOG_FILE_
#endif // _LOG_FILE_

#ifndef MAX_LOG_SIZE
#define MAX_LOG_SIZE 2048
#endif // MAX_LOG_SIZE

#ifdef _LOG_MONITOR_
namespace slog
{
enum ELogLevel
{
    LL_DEBUG    = 'D',
    LL_INFO     = 'I',
    LL_ERROR    = 'E',
    LL_FATAL    = 'F'
};
} // namespace slog

#ifndef SLOG
#define SLOG(level, content) \
    { \
        std::string strTmp(__FILE__); \
        std::string strRelativeFilePath = strTmp; \
        size_t iLastSlashPos = strTmp.rfind('/'); \
        if (iLastSlashPos != std::string::npos) \
        { \
            strRelativeFilePath = strTmp.substr(iLastSlashPos + 1); \
        } \
        std::printf("[%c]%s:%d] %s\n", level, strRelativeFilePath.c_str(), __LINE__, content); \
    } \

#endif // SLOG

#ifndef SLOG2
#define SLOG2(level, pattern, ...) \
    { \
        std::string strTmp(__FILE__); \
        std::string strRelativeFilePath = strTmp; \
        std::string strDataFilled = ""; \
        size_t iLastSlashPos = strTmp.rfind('/'); \
        util::Utils::LogBuilder(strDataFilled, pattern, ##__VA_ARGS__); \
        if (iLastSlashPos != std::string::npos) \
        { \
            strRelativeFilePath = strTmp.substr(iLastSlashPos + 1); \
        } \
        std::printf("[%c]%s:%d] %s\n", level, strRelativeFilePath.c_str(), __LINE__, strDataFilled.c_str()); \
    } \

#endif // SLOG2
#endif // _LOG_MONITOR_
